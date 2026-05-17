<template>
  <div id="app">
    <!-- 全局加载动画 -->
    <AppLoading 
      :show="isLoading" 
      :show-progress="showProgress"
      :progress="loadingProgress"
    />

    <!-- 几何背景容器 -->
    <div class="decor-container" ref="decorContainer"></div>
    
    <!-- 顶部公告栏 -->
    <AnnouncementBar @open-modal="showAnnouncement = true" />
    
    <!-- 头部导航 -->
    <TheHeader 
      :is-sidebar-open="showSidebar" 
      @toggle-sidebar="showSidebar = !showSidebar" 
    />
    
    <!-- 侧边栏 -->
    <Sidebar 
      :visible="showSidebar" 
      @close="showSidebar = false" 
    />
    
    <!-- 主要内容区域 -->
    <main class="main-content">
      <router-view />
    </main>
    
    <!-- 页脚 -->
    <Footer />
    
    <!-- 回到顶部按钮 -->
    <BackTop />
    
    <!-- 公告弹窗 -->
    <Modal 
      :visible="showAnnouncement" 
      @close="showAnnouncement = false" 
    />
  </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import AnnouncementBar from './components/common/AnnouncementBar.vue'
import TheHeader from './components/layout/TheHeader.vue'
import Sidebar from './components/layout/Sidebar.vue'
import Footer from './components/layout/Footer.vue'
import BackTop from './components/common/BackTop.vue'
import Modal from './components/common/Modal.vue'
import AppLoading from './components/common/AppLoading.vue'
import { loadAllData, registerCallbacks, loadingState } from './utils/dataLoader.js'

// 控制侧边栏显示
const showSidebar = ref(false)

// 控制公告弹窗显示
const showAnnouncement = ref(false)

// 几何背景容器
const decorContainer = ref(null)

// 加载状态
const isLoading = ref(true)
const showProgress = ref(false)
const loadingProgress = ref(0)

// 几何背景配置
const colors = ['#e8f4ff', '#f0ffee', '#fff0f5', '#f8f8ff', '#fff8e8', '#fef0e8', '#f8e8fe']

// 生成几何背景
const createShapes = () => {
  if (!decorContainer.value) return
  
  // 清空现有图形
  decorContainer.value.innerHTML = ''
  
  const isMobile = window.innerWidth < 768
  const shapeCount = isMobile ? 5 : 8
  const sizeRange = isMobile ? [50, 180] : [80, 280]
  
  for (let i = 0; i < shapeCount; i++) {
    const shape = document.createElement('div')
    shape.classList.add('decor-shape')
    
    const size = Math.floor(Math.random() * (sizeRange[1] - sizeRange[0])) + sizeRange[0]
    const top = Math.floor(Math.random() * 100)
    const left = Math.floor(Math.random() * 100)
    const rotate = Math.floor(Math.random() * 45)
    const radius = Math.random() > 0.5 ? '50%' : `${Math.floor(Math.random() * 20) + 5}px`
    const color = colors[Math.floor(Math.random() * colors.length)]
    const delay = Math.random() * 4
    
    shape.style.setProperty('--size', `${size}px`)
    shape.style.setProperty('--rotate', `${rotate}deg`)
    shape.style.setProperty('--radius', radius)
    shape.style.setProperty('--color', color)
    shape.style.setProperty('--delay', `${delay}s`)
    shape.style.top = `${top}%`
    shape.style.left = `${left}%`
    
    decorContainer.value.appendChild(shape)
  }
}

// 窗口大小变化时重新生成
const handleResize = () => {
  createShapes()
}

// 初始化应用
const initApp = async () => {
  // 注册回调函数
  registerCallbacks({
    onInitialLoad: (data) => {
      // 数据加载完成
      console.log('初始数据加载完成', data.fromCache ? '(来自缓存)' : '(来自网络)')
      // 短暂延迟以确保动画可见（首次访问时）
      if (!data.fromCache) {
        setTimeout(() => {
          isLoading.value = false
        }, 300)
      } else {
        // 使用缓存时几乎不需要显示加载动画
        isLoading.value = false
      }
    },
    onSilentUpdate: (data) => {
      // 静默更新完成，可以触发页面刷新或显示提示
      console.log('数据静默更新完成')
    },
    onError: (error) => {
      // 加载失败
      console.error('数据加载失败:', error)
      // 仍然关闭加载动画，显示错误内容
      isLoading.value = false
    }
  })

  // 加载所有数据
  await loadAllData(true)
}

// 模拟加载进度（可选，用于更好的用户体验）
let progressInterval = null

const startProgressAnimation = () => {
  showProgress.value = true
  let progress = 0
  
  progressInterval = setInterval(() => {
    if (progress < 90) {
      progress += Math.random() * 15
      if (progress > 90) progress = 90
      loadingProgress.value = Math.floor(progress)
    }
  }, 200)
}

const completeProgress = () => {
  if (progressInterval) {
    clearInterval(progressInterval)
    progressInterval = null
  }
  loadingProgress.value = 100
  showProgress.value = false
}

onMounted(() => {
  // 生成几何背景
  createShapes()
  window.addEventListener('resize', handleResize)
  
  // 开始加载进度动画
  startProgressAnimation()
  
  // 初始化应用并加载数据
  initApp().finally(() => {
    completeProgress()
  })
})

onUnmounted(() => {
  window.removeEventListener('resize', handleResize)
  if (progressInterval) {
    clearInterval(progressInterval)
  }
})
</script>

<style>
/* 几何背景 */
.decor-container {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  z-index: -1;
  pointer-events: none;
}

.decor-shape {
  position: absolute;
  opacity: 0.7;
  border-radius: var(--radius);
  background: var(--color);
  width: var(--size);
  height: var(--size);
  transform: rotate(var(--rotate));
  animation: float 12s ease-in-out infinite var(--delay);
}

@keyframes float {
  0% {
    transform: rotate(var(--rotate)) translateY(0px);
  }
  50% {
    transform: rotate(calc(var(--rotate) + 15deg)) translateY(-20px);
  }
  100% {
    transform: rotate(var(--rotate)) translateY(0px);
  }
}
</style>

<style scoped>
#app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  position: relative;
  z-index: 0;
}

.main-content {
  flex: 1;
}
</style>
