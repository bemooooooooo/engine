#pragma once
#include "Math.h"
#include "Vector3.h"
#include "Vector2.h"

// Forward declaration
struct Matrix4;

/**
 * @struct Matrix3
 * @brief A 3x3 matrix structure for mathematics and graphics operations.
 * 
 * Matrix3 represents a 3x3 matrix used for various transformations in 2D and 3D space,
 * including rotation, scaling, and shearing. It supports standard matrix operations
 * such as addition, subtraction, multiplication, transposition, and inversion.
 * Common uses:
 * - 2D transformations (rotation, scaling, shearing)
 *
 * - Normal matrix calculations in 3D graphics
 *
 * - Linear transformations in 3D space
 *
 * @note All components are stored as f32 (single-precision floats) for performance.
 * @note Matrix operations are defined as operator overloads for intuitive syntax.
 * @example
 * @code
 * Matrix3 matA;
 * Matrix3 matB;
 * Matrix3 matC = matA * matB;  // Matrix multiplication
 * Vector3 transformed = matA * Vector3(1.0f, 2.0f, 1.0f);
 * @endcode
 */
struct Matrix3 {
    union {
        f32 m[9];
        f32 mm[3][3];
        Vector3 columns[3];
    };
    
    Matrix3() {
        SetIdentity();
    }
    
    explicit Matrix3(const f32* data) {
        memcpy(m, data, 9 * sizeof(f32));
    }
    
    Matrix3(f32 m00, f32 m01, f32 m02,
            f32 m10, f32 m11, f32 m12,
            f32 m20, f32 m21, f32 m22) {
        m[0] = m00; m[1] = m01; m[2] = m02;
        m[3] = m10; m[4] = m11; m[5] = m12;
        m[6] = m20; m[7] = m21; m[8] = m22;
    }
    
    // Базовые операции
    Matrix3 operator+(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 9; ++i) {
            result.m[i] = m[i] + other.m[i];
        }
        return result;
    }
    
    Matrix3 operator-(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 9; ++i) {
            result.m[i] = m[i] - other.m[i];
        }
        return result;
    }
    
    Matrix3 operator*(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.mm[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k) {
                    result.mm[i][j] += mm[i][k] * other.mm[k][j];
                }
            }
        }
        return result;
    }
    
    Vector3 operator*(const Vector3& vec) const {
        return Vector3(
            m[0] * vec.x + m[1] * vec.y + m[2] * vec.z,
            m[3] * vec.x + m[4] * vec.y + m[5] * vec.z,
            m[6] * vec.x + m[7] * vec.y + m[8] * vec.z
        );
    }
    
    Matrix3& operator*=(const Matrix3& other) {
        *this = *this * other;
        return *this;
    }
    
    bool operator==(const Matrix3& other) const {
        for (int i = 0; i < 9; ++i) {
            if (!Math::Approximately(m[i], other.m[i])) return false;
        }
        return true;
    }
    
    f32 operator()(int row, int col) const {
        return mm[row][col];
    }
    
    f32& operator()(int row, int col) {
        return mm[row][col];
    }
    
    // Методы
    void SetIdentity() {
        static const f32 identity[9] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
        memcpy(m, identity, 9 * sizeof(f32));
    }
    
    Matrix3 Transposed() const {
        Matrix3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.mm[j][i] = mm[i][j];
            }
        }
        return result;
    }
    
    f32 Determinant() const {
        return m[0] * (m[4] * m[8] - m[5] * m[7]) -
               m[1] * (m[3] * m[8] - m[5] * m[6]) +
               m[2] * (m[3] * m[7] - m[4] * m[6]);
    }
    
    Matrix3 Inverted() const {
        Matrix3 result;
        f32 det = Determinant();
        
        if (Math::NearZero(det)) {
            return Identity();
        }
        
        f32 invDet = 1.0f / det;
        
        result.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
        result.m[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
        result.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
        
        result.m[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
        result.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
        result.m[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
        
        result.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
        result.m[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
        result.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;
        
        return result;
    }
    
    // Статические методы
    static Matrix3 Identity() {
        return Matrix3();
    }
    
    static Matrix3 Scale(const Vector2& scale) {
        Matrix3 result;
        result.m[0] = scale.x;
        result.m[4] = scale.y;
        return result;
    }
    
    static Matrix3 Rotation(f32 angle) {
        Matrix3 result;
        f32 c = Math::Cos(angle);
        f32 s = Math::Sin(angle);
        
        result.m[0] = c;
        result.m[1] = -s;
        result.m[3] = s;
        result.m[4] = c;
        
        return result;
    }
    
    static Matrix3 Translation(const Vector2& translation) {
        Matrix3 result;
        result.m[2] = translation.x;
        result.m[5] = translation.y;
        return result;
    }
    
    static Matrix3 FromMatrix4(const Matrix4& matrix4);
};

// Реализация FromMatrix4 требует полного определения Matrix4
// Включаем Matrix4.h здесь для реализации (Matrix4 уже включает Matrix3, но это безопасно
// так как мы включаем его после объявления Matrix3)
#include "Matrix4.h"

inline Matrix3 Matrix3::FromMatrix4(const Matrix4& matrix4) {
    Matrix3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mm[i][j] = matrix4(i, j);
        }
    }
    return result;
}