#include "Engine.h"

#include "Graphics/Graphics.h"

Engine* Engine::s_Instance_ = nullptr;

Engine::Engine() {
  if (s_Instance_) {
    ENGINE_CRITICAL << "Engine instance already exists!";
    return;
  }
  s_Instance_ = this;
  ENGINE_INFO << "Engine constructor";
}

Engine::~Engine() {
  ENGINE_INFO << "Engine destructor";
  Shutdown();
  s_Instance_ = nullptr;
}

bool Engine::Initialize() {
  ENGINE_INFO << "Initializing engine...";

  Log::Init();

  m_Window_ = CreateScope<Window>();
  if (!m_Window_->Initialize()) {
    ENGINE_CRITICAL << "Failed to initialize window";
    return false;
  }

  m_Window_->SetEventCallback(
      [this](Event& event) { this->OnEvent(event); });

  m_isRunning_ = true;

  OnInitialize();

  ENGINE_INFO << "Engine initialized successfully";
  return true;
}

void Engine::Shutdown() {
  if (!m_isRunning_) return;
  ENGINE_INFO << "Shutting down engine...";

  OnShutdown();

  if (m_Window_) {
    m_Window_->Shutdown();
  }

  m_isRunning_ = false;
  ENGINE_INFO << "Engine shutdown complete";
}

void Engine::Run() {
    if (!Initialize()) {
        ENGINE_CRITICAL << "Failed to initialize engine!";
        return;
    }

    ENGINE_INFO << "Starting main loop";
    
    // Initialize input system
    Input::Init();
    
    // Main loop
    while (m_isRunning_ && !m_Window_->ShouldClose()) {
        // Update timer and calculate delta time
        m_DeltaTime_ = m_Timer_.Tick();
        
        // Calculate FPS
        m_FrameCount_++;
        m_FPSTimer_ += m_DeltaTime_;
        if (m_FPSTimer_ >= 1.0f) {
            m_FPS_ = static_cast<f32>(m_FrameCount_) / m_FPSTimer_;
            m_FrameCount_ = 0;
            m_FPSTimer_ = 0.0f;
            
            ENGINE_TRACE << "FPS: " << m_FPS_ << ", DeltaTime: " << m_DeltaTime_;
        }
        

        ProcessInput();
        
        // Update game logic
        Update(m_DeltaTime_);
        
        // Render the scene
        if (!m_isMinimized) {
            Render();
        }
        
        // Update window (poll events + swap buffers)
        m_Window_->Update();
    }
    
    Shutdown();
}

void Engine::ProcessInput() {
    if (Input::GetKey(Key::Escape)) {
        m_isRunning_ = false;
    }
    
    // TODO додумать обработку входных данных
    if (Input::GetKeyDown(Key::Space)) {
        ENGINE_INFO << "Space pressed!";
    }
    
    if (Input::GetMouseButtonDown(Mouse::Left)) {
        ENGINE_INFO << "Left mouse button clicked at: " << Input::GetMousePosition().x << ", " << Input::GetMousePosition().y;
    }
}

void Engine::Update(f32 deltaTime) {
  OnUpdate(deltaTime);
}

void Engine::Render(){
  m_Window_->BeginFrame();

  OnRender();

  m_Window_->EndFrame();
}