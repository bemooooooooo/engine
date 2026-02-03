#pragma once
#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"
#include <string>
#include <sstream>
#include <iomanip>

// Forward declaration для избежания циклической зависимости
struct Matrix3;

struct Matrix4 {
  union {
    f32 m[16];
    f32 mm[4][4];
    Vector4 columns[4];
  };

  Matrix4() { SetIdentity(); }

  explicit Matrix4(const f32* data) { memcpy(m, data, 16 * sizeof(f32)); }

  Matrix4(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12,
          f32 m13, f32 m20, f32 m21, f32 m22, f32 m23, f32 m30, f32 m31,
          f32 m32, f32 m33) {
    m[0] = m00;
    m[1] = m01;
    m[2] = m02;
    m[3] = m03;
    m[4] = m10;
    m[5] = m11;
    m[6] = m12;
    m[7] = m13;
    m[8] = m20;
    m[9] = m21;
    m[10] = m22;
    m[11] = m23;
    m[12] = m30;
    m[13] = m31;
    m[14] = m32;
    m[15] = m33;
  }

  // Базовые операции
  Matrix4 operator+(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
      result.m[i] = m[i] + other.m[i];
    }
    return result;
  }

  Matrix4 operator-(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
      result.m[i] = m[i] - other.m[i];
    }
    return result;
  }

  Matrix4 operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.mm[i][j] = 0.0f;
        for (int k = 0; k < 4; ++k) {
          result.mm[i][j] += mm[i][k] * other.mm[k][j];
        }
      }
    }
    return result;
  }

  Vector4 operator*(const Vector4& vec) const {
    return Vector4(
        m[0] * vec.x + m[1] * vec.y + m[2] * vec.z + m[3] * vec.w,
        m[4] * vec.x + m[5] * vec.y + m[6] * vec.z + m[7] * vec.w,
        m[8] * vec.x + m[9] * vec.y + m[10] * vec.z + m[11] * vec.w,
        m[12] * vec.x + m[13] * vec.y + m[14] * vec.z + m[15] * vec.w);
  }

  Vector3 TransformPoint(const Vector3& point) const {
    Vector4 result = *this * Vector4(point, 1.0f);
    return result.Homogenized();
  }

  Vector3 TransformDirection(const Vector3& direction) const {
    Vector4 result = *this * Vector4(direction, 0.0f);
    return result.XYZ();
  }

  Matrix4& operator*=(const Matrix4& other) {
    *this = *this * other;
    return *this;
  }

  bool operator==(const Matrix4& other) const {
    for (int i = 0; i < 16; ++i) {
      if (!Math::Approximately(m[i], other.m[i])) return false;
    }
    return true;
  }

  bool operator!=(const Matrix4& other) const { return !(*this == other); }

  f32 operator()(int row, int col) const { return mm[row][col]; }

  f32& operator()(int row, int col) { return mm[row][col]; }

  // Методы
  void SetIdentity() {
    static const f32 identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};
    memcpy(m, identity, 16 * sizeof(f32));
  }

  Matrix4 Transposed() const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.mm[j][i] = mm[i][j];
      }
    }
    return result;
  }

  f32 Determinant() const {
    f32 det = 0.0f;
    det += m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14]) -
                   m[6] * (m[9] * m[15] - m[11] * m[13]) +
                   m[7] * (m[9] * m[14] - m[10] * m[13]));
    det -= m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14]) -
                   m[6] * (m[8] * m[15] - m[11] * m[12]) +
                   m[7] * (m[8] * m[14] - m[10] * m[12]));
    det += m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13]) -
                   m[5] * (m[8] * m[15] - m[11] * m[12]) +
                   m[7] * (m[8] * m[13] - m[9] * m[12]));
    det -= m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13]) -
                   m[5] * (m[8] * m[14] - m[10] * m[12]) +
                   m[6] * (m[8] * m[13] - m[9] * m[12]));
    return det;
  }

  Matrix4 Inverted() const {
    Matrix4 result;
    f32 det = Determinant();

    if (Math::NearZero(det)) {
      return Identity();
    }

    f32 invDet = 1.0f / det;

    result.m[0] = invDet * (m[5] * (m[10] * m[15] - m[11] * m[14]) -
                            m[6] * (m[9] * m[15] - m[11] * m[13]) +
                            m[7] * (m[9] * m[14] - m[10] * m[13]));

    result.m[1] = -invDet * (m[1] * (m[10] * m[15] - m[11] * m[14]) -
                             m[2] * (m[9] * m[15] - m[11] * m[13]) +
                             m[3] * (m[9] * m[14] - m[10] * m[13]));

    result.m[2] = invDet * (m[1] * (m[6] * m[15] - m[7] * m[14]) -
                            m[2] * (m[5] * m[15] - m[7] * m[13]) +
                            m[3] * (m[5] * m[14] - m[6] * m[13]));

    result.m[3] = -invDet * (m[1] * (m[6] * m[11] - m[7] * m[10]) -
                             m[2] * (m[5] * m[11] - m[7] * m[9]) +
                             m[3] * (m[5] * m[10] - m[6] * m[9]));

    result.m[4] = -invDet * (m[4] * (m[10] * m[15] - m[11] * m[14]) -
                             m[6] * (m[8] * m[15] - m[11] * m[12]) +
                             m[7] * (m[8] * m[14] - m[10] * m[12]));

    result.m[5] = invDet * (m[0] * (m[10] * m[15] - m[11] * m[14]) -
                            m[2] * (m[8] * m[15] - m[11] * m[12]) +
                            m[3] * (m[8] * m[14] - m[10] * m[12]));

    result.m[6] = -invDet * (m[0] * (m[6] * m[15] - m[7] * m[14]) -
                             m[2] * (m[4] * m[15] - m[7] * m[12]) +
                             m[3] * (m[4] * m[14] - m[6] * m[12]));

    result.m[7] = invDet * (m[0] * (m[6] * m[11] - m[7] * m[10]) -
                            m[2] * (m[4] * m[11] - m[7] * m[8]) +
                            m[3] * (m[4] * m[10] - m[6] * m[8]));

    result.m[8] = invDet * (m[4] * (m[9] * m[15] - m[11] * m[13]) -
                            m[5] * (m[8] * m[15] - m[11] * m[12]) +
                            m[7] * (m[8] * m[13] - m[9] * m[12]));

    result.m[9] = -invDet * (m[0] * (m[9] * m[15] - m[11] * m[13]) -
                             m[1] * (m[8] * m[15] - m[11] * m[12]) +
                             m[3] * (m[8] * m[13] - m[9] * m[12]));

    result.m[10] = invDet * (m[0] * (m[5] * m[15] - m[7] * m[13]) -
                             m[1] * (m[4] * m[15] - m[7] * m[12]) +
                             m[3] * (m[4] * m[13] - m[5] * m[12]));

    result.m[11] = -invDet * (m[0] * (m[5] * m[11] - m[7] * m[9]) -
                              m[1] * (m[4] * m[11] - m[7] * m[8]) +
                              m[3] * (m[4] * m[9] - m[5] * m[8]));

    result.m[12] = -invDet * (m[4] * (m[9] * m[14] - m[10] * m[13]) -
                              m[5] * (m[8] * m[14] - m[10] * m[12]) +
                              m[6] * (m[8] * m[13] - m[9] * m[12]));

    result.m[13] = invDet * (m[0] * (m[9] * m[14] - m[10] * m[13]) -
                             m[1] * (m[8] * m[14] - m[10] * m[12]) +
                             m[2] * (m[8] * m[13] - m[9] * m[12]));

    result.m[14] = -invDet * (m[0] * (m[5] * m[14] - m[6] * m[13]) -
                              m[1] * (m[4] * m[14] - m[6] * m[12]) +
                              m[2] * (m[4] * m[13] - m[5] * m[12]));

    result.m[15] = invDet * (m[0] * (m[5] * m[10] - m[6] * m[9]) -
                             m[1] * (m[4] * m[10] - m[6] * m[8]) +
                             m[2] * (m[4] * m[9] - m[5] * m[8]));

    return result;
  }

  static Matrix4 Identity() { return Matrix4(); }
  
  // Получить нормальную матрицу (3x3) для шейдеров
  // Реализация после включения Matrix3.h
  Matrix3 GetNormalMatrix() const;

  static Matrix4 Translation(const Vector3& translation) {
    Matrix4 result;
    result.m[3] = translation.x;
    result.m[7] = translation.y;
    result.m[11] = translation.z;
    return result;
  }

  static Matrix4 Scale(const Vector3& scale) {
    Matrix4 result;
    result.m[0] = scale.x;
    result.m[5] = scale.y;
    result.m[10] = scale.z;
    return result;
  }

  static Matrix4 RotationX(f32 angle) {
    Matrix4 result;
    f32 c = Math::Cos(angle);
    f32 s = Math::Sin(angle);

    result.m[5] = c;
    result.m[6] = -s;
    result.m[9] = s;
    result.m[10] = c;

    return result;
  }

  static Matrix4 RotationY(f32 angle) {
    Matrix4 result;
    f32 c = Math::Cos(angle);
    f32 s = Math::Sin(angle);

    result.m[0] = c;
    result.m[2] = s;
    result.m[8] = -s;
    result.m[10] = c;

    return result;
  }

  static Matrix4 RotationZ(f32 angle) {
    Matrix4 result;
    f32 c = Math::Cos(angle);
    f32 s = Math::Sin(angle);

    result.m[0] = c;
    result.m[1] = -s;
    result.m[4] = s;
    result.m[5] = c;

    return result;
  }

  static Matrix4 Rotation(const Vector3& axis, f32 angle) {
    Matrix4 result;
    Vector3 n = axis.Normalized();
    f32 c = Math::Cos(angle);
    f32 s = Math::Sin(angle);
    f32 t = 1.0f - c;

    result.m[0] = t * n.x * n.x + c;
    result.m[1] = t * n.x * n.y - s * n.z;
    result.m[2] = t * n.x * n.z + s * n.y;

    result.m[4] = t * n.x * n.y + s * n.z;
    result.m[5] = t * n.y * n.y + c;
    result.m[6] = t * n.y * n.z - s * n.x;

    result.m[8] = t * n.x * n.z - s * n.y;
    result.m[9] = t * n.y * n.z + s * n.x;
    result.m[10] = t * n.z * n.z + c;

    return result;
  }

  // Проекционные матрицы
  static Matrix4 Orthographic(f32 left, f32 right, f32 bottom, f32 top,
                              f32 near, f32 far) {
    Matrix4 result;

    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);

    result.m[3] = -(right + left) / (right - left);
    result.m[7] = -(top + bottom) / (top - bottom);
    result.m[11] = -(far + near) / (far - near);

    return result;
  }

  static Matrix4 Perspective(f32 fov, f32 aspect, f32 near, f32 far) {
    Matrix4 result;
    f32 tanHalfFov = Math::Tan(fov * 0.5f);
    
    // Исправляем матрицу перспективной проекции для OpenGL
    result.m[0] = 1.0f / (aspect * tanHalfFov);  // scale x
    result.m[5] = 1.0f / tanHalfFov;             // scale y
    result.m[10] = -(far + near) / (far - near); // scale z
    result.m[11] = -(2.0f * far * near) / (far - near); // translation z
    result.m[14] = -1.0f;                              // perspective divide
    result.m[15] = 0.0f;                         // w = 0 для матрицы проекции

    return result;
  }

  static Matrix4 LookAt(const Vector3& eye, const Vector3& target,
                        const Vector3& up) {
    // Правильная система координат для OpenGL
    Vector3 z = (eye - target).Normalized(); // Инвертируем направление взгляда для OpenGL
    Vector3 x = Vector3::Cross(up, z).Normalized();
    Vector3 y = Vector3::Cross(z, x);

    Matrix4 result;
    // Устанавливаем базисные векторы
    result.m[0] = x.x;
    result.m[1] = x.y;
    result.m[2] = x.z;
    result.m[4] = y.x;
    result.m[5] = y.y;
    result.m[6] = y.z;
    result.m[8] = z.x;
    result.m[9] = z.y;
    result.m[10] = z.z;

    result.m[3] = -Vector3::Dot(x, eye);
    result.m[7] = -Vector3::Dot(y, eye);
    result.m[11] = -Vector3::Dot(z, eye);

    return result;
  }

  std::string ToString() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(4);
    for (int row = 0; row < 4; ++row) {
      ss << "[ ";
      for (int col = 0; col < 4; ++col) {
        ss << std::setw(8) << mm[col][row];
        if (col < 3) ss << ", ";
      }
      ss << " ]";
      if (row < 3) ss << "\n";
    }
    return ss.str();
  }
};

// Включаем Matrix3.h после объявления Matrix4 для реализации GetNormalMatrix
#include "Matrix3.h"

inline Matrix3 Matrix4::GetNormalMatrix() const {
  // Берем верхнюю левую 3x3 часть матрицы, инвертируем и транспонируем
  Matrix3 upperLeft(
    m[0], m[1], m[2],
    m[4], m[5], m[6],
    m[8], m[9], m[10]
  );
  return upperLeft.Inverted().Transposed();
}


