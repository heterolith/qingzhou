<template>
  <Transition name="loading-fade">
    <div v-if="show" class="app-loading">
      <div class="loading-content">
        <!-- Logo/标题 -->
        <div class="loading-logo">
          <div class="logo-icon">
            <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
              <circle cx="50" cy="50" r="40" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" class="circle circle-1"/>
              <circle cx="50" cy="50" r="30" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" class="circle circle-2"/>
              <circle cx="50" cy="50" r="20" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" class="circle circle-3"/>
              <circle cx="50" cy="50" r="8" fill="currentColor" class="center-dot"/>
            </svg>
          </div>
          <div class="logo-text">
            <span class="logo-main">晴宙</span>
            <span class="logo-sub">QingZhou</span>
          </div>
        </div>

        <!-- 加载动画 -->
        <div class="loading-spinner">
          <div class="spinner-ring">
            <div class="ring"></div>
            <div class="ring"></div>
            <div class="ring"></div>
          </div>
        </div>

        <!-- 加载提示 -->
        <div class="loading-text">
          <span>{{ loadingText }}</span>
        </div>

        <!-- 进度条（可选） -->
        <div v-if="showProgress" class="loading-progress">
          <div class="progress-bar">
            <div class="progress-fill" :style="{ width: progress + '%' }"></div>
          </div>
          <div class="progress-text">{{ progress }}%</div>
        </div>
      </div>

      <!-- 装饰背景 -->
      <div class="loading-bg">
        <div class="bg-shape shape-1"></div>
        <div class="bg-shape shape-2"></div>
        <div class="bg-shape shape-3"></div>
        <div class="bg-shape shape-4"></div>
      </div>
    </div>
  </Transition>
</template>

<script setup>
import { ref, watch, onMounted } from 'vue'

const props = defineProps({
  show: {
    type: Boolean,
    default: false
  },
  showProgress: {
    type: Boolean,
    default: false
  },
  progress: {
    type: Number,
    default: 0
  },
  text: {
    type: String,
    default: ''
  }
})

// 加载提示文字
const loadingTexts = [
  '正在加载内容...',
  '加载中...',
  '稍等片刻...',
  '马上就好...'
]

const loadingIndex = ref(0)
const loadingText = ref(loadingTexts[0])

// 如果传入了自定义文字，使用自定义的
watch(() => props.text, (newText) => {
  if (newText) {
    loadingText.value = newText
  }
}, { immediate: true })

// 轮换加载提示文字
let textInterval = null

onMounted(() => {
  textInterval = setInterval(() => {
    if (!props.text) {
      loadingIndex.value = (loadingIndex.value + 1) % loadingTexts.length
      loadingText.value = loadingTexts[loadingIndex.value]
    }
  }, 2000)
})

// 组件卸载时清除定时器
import { onUnmounted } from 'vue'
onUnmounted(() => {
  if (textInterval) {
    clearInterval(textInterval)
  }
})
</script>

<style scoped>
.app-loading {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  z-index: 99999;
  display: flex;
  align-items: center;
  justify-content: center;
  background: var(--bg-primary);
  overflow: hidden;
}

.loading-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  z-index: 2;
  position: relative;
}

/* Logo 样式 */
.loading-logo {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-bottom: 40px;
}

.logo-icon {
  width: 80px;
  height: 80px;
  color: var(--accent);
  margin-bottom: 16px;
}

.logo-icon svg {
  width: 100%;
  height: 100%;
}

.circle {
  transform-origin: center;
  animation: pulse 2s ease-in-out infinite;
}

.circle-1 {
  animation-delay: 0s;
}

.circle-2 {
  animation-delay: 0.3s;
}

.circle-3 {
  animation-delay: 0.6s;
}

.center-dot {
  animation: glow 2s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% {
    opacity: 0.4;
    transform: scale(0.95);
  }
  50% {
    opacity: 1;
    transform: scale(1);
  }
}

@keyframes glow {
  0%, 100% {
    opacity: 0.6;
  }
  50% {
    opacity: 1;
  }
}

.logo-text {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
}

.logo-main {
  font-size: 2rem;
  font-weight: 700;
  color: var(--text-primary);
  letter-spacing: 0.1em;
}

.logo-sub {
  font-size: 0.875rem;
  color: var(--text-muted);
  letter-spacing: 0.2em;
  text-transform: uppercase;
}

/* 加载动画 */
.loading-spinner {
  margin-bottom: 24px;
}

.spinner-ring {
  position: relative;
  width: 60px;
  height: 60px;
}

.ring {
  position: absolute;
  top: 50%;
  left: 50%;
  width: 100%;
  height: 100%;
  border: 3px solid transparent;
  border-top-color: var(--accent);
  border-radius: 50%;
  animation: spin 1.2s cubic-bezier(0.5, 0, 0.5, 1) infinite;
}

.ring:nth-child(1) {
  animation-delay: -0.45s;
}

.ring:nth-child(2) {
  width: 80%;
  height: 80%;
  top: 10%;
  left: 10%;
  animation-delay: -0.3s;
  border-top-color: var(--highlight-color);
}

.ring:nth-child(3) {
  width: 60%;
  height: 60%;
  top: 20%;
  left: 20%;
  animation-delay: -0.15s;
  border-top-color: var(--notice-color);
}

@keyframes spin {
  0% {
    transform: translate(-50%, -50%) rotate(0deg);
  }
  100% {
    transform: translate(-50%, -50%) rotate(360deg);
  }
}

/* 加载文字 */
.loading-text {
  font-size: 0.875rem;
  color: var(--text-muted);
  margin-top: 16px;
  min-height: 1.5em;
}

/* 进度条 */
.loading-progress {
  width: 200px;
  margin-top: 20px;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 8px;
}

.progress-bar {
  width: 100%;
  height: 4px;
  background: var(--border-color);
  border-radius: 2px;
  overflow: hidden;
}

.progress-fill {
  height: 100%;
  background: linear-gradient(90deg, var(--accent), var(--highlight-color));
  border-radius: 2px;
  transition: width 0.3s ease;
}

.progress-text {
  font-size: 0.75rem;
  color: var(--text-muted);
}

/* 装饰背景 */
.loading-bg {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  overflow: hidden;
  z-index: 1;
  opacity: 0.5;
}

.bg-shape {
  position: absolute;
  border-radius: 50%;
  opacity: 0.15;
}

.shape-1 {
  width: 300px;
  height: 300px;
  background: var(--accent);
  top: -100px;
  right: -100px;
  animation: float-shape 8s ease-in-out infinite;
}

.shape-2 {
  width: 200px;
  height: 200px;
  background: var(--highlight-color);
  bottom: -50px;
  left: -50px;
  animation: float-shape 10s ease-in-out infinite reverse;
}

.shape-3 {
  width: 150px;
  height: 150px;
  background: var(--notice-color);
  top: 30%;
  left: 10%;
  animation: float-shape 12s ease-in-out infinite;
  animation-delay: -4s;
}

.shape-4 {
  width: 180px;
  height: 180px;
  background: var(--accent);
  bottom: 20%;
  right: 15%;
  animation: float-shape 9s ease-in-out infinite;
  animation-delay: -2s;
}

@keyframes float-shape {
  0%, 100% {
    transform: translate(0, 0) scale(1);
  }
  33% {
    transform: translate(30px, -30px) scale(1.05);
  }
  66% {
    transform: translate(-20px, 20px) scale(0.95);
  }
}

/* 淡入淡出过渡 */
.loading-fade-enter-active,
.loading-fade-leave-active {
  transition: opacity 0.6s ease, visibility 0.6s ease;
}

.loading-fade-enter-from,
.loading-fade-leave-to {
  opacity: 0;
  visibility: hidden;
}

/* 暗色模式适配 */
.dark .logo-icon {
  color: var(--accent);
}

.dark .bg-shape {
  opacity: 0.2;
}
</style>
