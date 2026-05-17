<template>
  <div class="resource-container" v-if="resource">
    <!-- 返回按钮 -->
    <div class="max-w-4xl mx-auto px-4 sm:px-6 pt-6">
      <router-link to="/resources" class="inline-flex items-center text-gray-500 hover:text-blue-500 transition-colors">
        <svg class="w-5 h-5 mr-1" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
          <path d="M256 120l-45.25 45.25L458.75 512 210.75 758.75 256 804l320-320z"/>
        </svg>
        返回列表
      </router-link>
    </div>
    
    <!-- 卡片容器 -->
    <div class="detail-card max-w-4xl mx-auto mt-6 mb-10">
      <!-- 主图 -->
      <div v-if="resource.image">
        <div class="w-full aspect-video bg-gray-200 overflow-hidden rounded-t-[25px]">
          <img :src="resource.image" :alt="resource.title" class="w-full h-full object-cover">
        </div>
      </div>
      
      <!-- 基础信息 -->
      <header class="px-4 sm:px-8 pt-2 pb-6 border-b border-gray-100 text-center">
        <!-- 标题 -->
        <h1 class="text-2xl sm:text-3xl font-bold text-gray-900 mb-2">{{ resource.title }}</h1>
        
        <!-- 一句话描述 -->
        <p class="text-gray-500 mb-4">{{ resource.description }}</p>
        
        <!-- 警告信息 -->
        <p v-if="resource.warning" class="text-sm text-red-500 font-medium mb-4">{{ resource.warning }}</p>
        
        <!-- 作者·日期·AI辅助标签 -->
        <div class="flex justify-center gap-8 text-sm text-gray-500 mb-4 flex-wrap">
          <p class="flex items-center gap-1.5">
            <svg class="w-4 h-4 text-orange-500" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
              <path d="M512 1004.307692C240.443077 1004.307692 19.692308 783.556923 19.692308 512 19.692308 240.64 240.443077 19.692308 512 19.692308s492.307692 220.750769 492.307692 492.307692-220.750769 492.307692-492.307692 492.307692z m0-945.230769C262.301538 59.076923 59.076923 262.301538 59.076923 512s203.224615 452.923077 452.923077 452.923077 452.923077-203.224615 452.923077-452.923077S761.698462 59.076923 512 59.076923z"/>
              <path d="M512 609.870769c-108.504615 0-196.923077-88.418462-196.923077-196.923077s88.418462-196.923077 196.923077-196.923077 196.923077 88.418462 196.923077 196.923077-88.418462 196.923077-196.923077 196.923077z m0-354.461538c-86.843077 0-157.538462 70.695385-157.538462 157.538461s70.695385 157.538462 157.538462 157.538462 157.538462-70.695385 157.538462-157.538462-70.695385-157.538462-157.538462-157.538461z"/>
              <path d="M173.095385 859.963077c-1.969231 0-3.938462-0.196923-5.71077-0.787692a19.633231 19.633231 0 0 1-12.996923-24.615385c48.64-157.932308 192.393846-263.876923 357.612308-263.876923s308.972308 105.944615 357.612308 263.876923a19.633231 19.633231 0 0 1-12.996923 24.615385 19.633231 19.633231 0 0 1-24.615385-12.996923c-43.52-141.587692-172.110769-236.307692-320-236.307693S235.52 704.787692 192 845.981538c-2.56 8.467692-10.436923 13.981538-18.904615 13.981539z"/>
            </svg>
            {{ resource.author || '晴宙' }}
          </p>
          <p class="flex items-center gap-1.5">
            <svg class="w-4 h-4 text-slate-400" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
              <path d="M802.4576 896H233.472A115.6608 115.6608 0 0 1 117.76 780.4928V318.976a115.6608 115.6608 0 0 1 115.712-115.5072h41.8304a30.72 30.72 0 0 1 0 61.44H233.472A54.1696 54.1696 0 0 0 179.2 318.976v461.4656A54.1696 54.1696 0 0 0 233.472 834.56h568.9856a54.1696 54.1696 0 0 0 54.0672-54.0672V318.976a54.1696 54.1696 0 0 0-54.0672-54.0672h-38.7072a30.72 30.72 0 0 1 0-61.44h38.7072A115.6608 115.6608 0 0 1 918.016 318.976v461.4656A115.6608 115.6608 0 0 1 802.4576 896z"/>
              <path d="M610.7136 264.9088h-189.44a30.72 30.72 0 0 1 0-61.44h189.44a30.72 30.72 0 1 1 0 61.44zM877.9264 482.304H158.0032a30.72 30.72 0 0 1 0-61.44h719.9232a30.72 30.72 0 0 1 0 61.44zM347.3408 346.7264a30.72 30.72 0 0 1-30.72-30.72V166.4a30.72 30.72 0 0 1 61.44 0v149.6064a30.72 30.72 0 0 1-30.72 30.72zM688.5888 346.7264a30.72 30.72 0 0 1-30.72-30.72V166.4a30.72 30.72 0 0 1 61.44 0v149.6064a30.72 30.72 0 0 1-30.72 30.72z"/>
            </svg>
            {{ resource.time }}
          </p>
          <span class="px-2.5 py-0.5 rounded-[20px] text-xs bg-gray-200 text-gray-800">AI辅助生成</span>
        </div>
        
        <!-- 标签行 -->
        <div class="flex justify-center gap-2 mb-4 flex-wrap">
          <span 
            v-for="cat in categories" 
            :key="cat" 
            class="px-2.5 py-0.5 border rounded-[40px] text-sm text-gray-700"
            :style="{ borderColor: getCategoryColor(cat) }"
          >{{ cat }}</span>
        </div>
        
        <!-- 设备支持 -->
        <div v-if="resource.devices && resource.devices.length > 0" class="flex items-center justify-center gap-1 text-sm text-gray-400 mb-4">
          <template v-for="(device, index) in resource.devices" :key="device">
            <span v-if="index > 0">·</span>
            <span v-html="getDeviceIcon(device)"></span>
            <span>{{ getDeviceName(device) }}</span>
          </template>
        </div>
        
        <!-- 星级评分 -->
        <div v-if="resource.rating" class="flex items-center justify-center gap-2 mb-4">
          <div class="flex gap-0.5">
            <svg 
              v-for="i in 5" 
              :key="i"
              class="w-5 h-5" 
              :class="i <= Math.floor(resource.rating) ? 'text-yellow-400' : 'text-gray-300'"
              viewBox="0 0 1024 1024" 
              xmlns="http://www.w3.org/2000/svg"
            >
              <path d="M507.52 51.2l142.464 288.256 318.72 46.336-230.656 224.768 54.4 317.44L512 775.68 217.728 927.872l54.4-317.44L45.92 385.792l318.976-46.08L507.52 51.2z" fill="currentColor"/>
            </svg>
          </div>
          <span class="text-sm text-gray-600">{{ resource.rating }}</span>
        </div>
        
        <!-- 操作按钮 -->
        <div class="flex items-center justify-center gap-3 mt-4">
          <a 
            v-if="resource.official_url" 
            :href="resource.official_url" 
            target="_blank"
            class="px-6 py-2.5 bg-blue-500 text-white rounded-[40px] text-sm font-medium hover:bg-blue-600 transition-colors"
          >访问官网</a>
          <a 
            v-if="resource.download_url" 
            :href="resource.download_url" 
            target="_blank"
            class="px-6 py-2.5 border border-gray-300 text-gray-700 rounded-[40px] text-sm font-medium hover:bg-gray-50 transition-colors"
          >下载</a>
        </div>
      </header>
      
      <!-- 正文内容 -->
      <div class="px-4 sm:px-8 pt-6 pb-10">
        <div class="resource-content" v-html="resource.content"></div>
        
        <!-- 底部 -->
        <footer class="pt-6 border-t border-gray-200 text-sm text-gray-500 text-center mt-8">
          <p>© 2026 {{ resource.author || '晴宙' }} · 保留全部著作权利</p>
        </footer>
      </div>
    </div>
    
    <!-- 顶部吸顶横向导航条 + SVG 描边进度条 -->
    <nav id="top-nav" class="fixed left-0 right-0 z-50 bg-white rounded-[40px] shadow-sm mx-6 sm:mx-16 md:max-w-4xl md:mx-auto px-4 py-2" style="top: 115px;" v-if="navItems.length > 0">
      <!-- SVG 描边进度条 -->
      <svg ref="progressSvg" class="pointer-events-none absolute" style="top:0;left:0;width:100%;height:100%;z-index:10;overflow:visible;">
        <path ref="progressBgPath" fill="none" stroke="#e5e7eb" stroke-width="1"/>
        <path ref="progressPath" fill="none" stroke="#60a5fa" stroke-width="2" stroke-linecap="round"/>
      </svg>
      <div ref="navWrapper" class="relative flex items-center justify-center overflow-x-auto hide-scrollbar">
        <span class="text-gray-300">|</span>
        <template v-for="(item, index) in navItems" :key="item.href">
          <a 
            :href="item.href"
            :ref="el => { if (el) navItemRefs[index] = el }"
            class="nav-item text-sm text-gray-500 py-2 px-3 whitespace-nowrap"
            :class="{ active: activeIndex === index }"
            @click.prevent="scrollToSection(item.href)"
          >
            {{ item.label }}
          </a>
          <span class="text-gray-300">|</span>
        </template>
        <div ref="navIndicator" id="nav-indicator" class="nav-indicator"></div>
      </div>
    </nav>
  </div>
  
  <!-- 加载状态 -->
  <div class="flex items-center justify-center min-h-screen" v-else>
    <div class="text-gray-500">{{ error || '加载中...' }}</div>
  </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted, nextTick, computed } from 'vue'
import { useRoute } from 'vue-router'
import { fetchResourceById } from '../utils/dataLoader.js'

const route = useRoute()
const resource = ref(null)
const navItems = ref([])
const error = ref(null)

// DOM 引用
const navWrapper = ref(null)
const navIndicator = ref(null)
const navItemRefs = ref([])
const activeIndex = ref(0)

// 进度条相关
const progressSvg = ref(null)
const progressBgPath = ref(null)
const progressPath = ref(null)
const perimeter = ref(0)

// 标签颜色映射
const categoryColors = ['#f97316', '#3b82f6', '#22c55e', '#a855f7', '#ec4899', '#14b8a6', '#f59e0b']

// 计算分类数组
const categories = computed(() => {
  if (!resource.value?.category) return []
  return Array.isArray(resource.value.category) 
    ? resource.value.category 
    : resource.value.category.split(',').map(c => c.trim()).filter(Boolean)
})

// 获取分类颜色
function getCategoryColor(category) {
  const index = categories.value.indexOf(category)
  return categoryColors[index % categoryColors.length]
}

// 获取设备图标
function getDeviceIcon(device) {
  const icons = {
    'mobile': '<svg class="w-4 h-4 inline-block text-gray-400" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg"><path d="M740.16 960H283.84c-56.63 0-102.71-46.07-102.71-102.71V166.71C181.14 110.07 227.21 64 283.84 64h456.31c56.63 0 102.71 46.07 102.71 102.71V857.3c0 56.63-46.07 102.7-102.7 102.7zM283.84 132.47c-18.88 0-34.24 15.36-34.24 34.24V857.3c0 18.88 15.36 34.24 34.24 34.24h456.31c18.88 0 34.24-15.36 34.24-34.24V166.71c0-18.88-15.36-34.24-34.24-34.24H283.84z" fill="currentColor"/><path d="M578.36 273.47H445.64c-2.21 0-4-1.79-4-4v-65.25c0-2.21 1.79-4 4-4h132.73c2.21 0 4 1.79 4 4v65.25c-0.01 2.21-1.8 4-4.01 4zM548.41 834.28H475.6c-2.21 0-4-1.79-4-4v-65.25c0-2.21 1.79-4 4-4h72.81c2.21 0 4 1.79 4 4v65.25c0 2.21-1.79 4-4 4z" fill="currentColor"/></svg>',
    'tablet': '<svg class="w-4 h-4 inline-block text-gray-400" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg"><path d="M832 64c35.3456 0 64 28.6544 64 64v768c0 35.3456-28.6544 64-64 64H192c-35.3456 0-64-28.6544-64-64V128c0-35.3456 28.6544-64 64-64h640z m0 48H192c-8.8368 0-16 7.1632-16 16v768c0 8.8368 7.1632 16 16 16h640c8.8368 0 16-7.1632 16-16V128c0-8.8368-7.1632-16-16-16zM512 768c26.5104 0 48 21.4896 48 48 0 26.5104-21.4896 48-48 48-26.5104 0-48-21.4896-48-48 0-26.5104 21.4896-48 48-48z m104-608c13.2544 0 24 10.7456 24 24 0 13.2544-10.7456 24-24 24H408c-13.2544 0-24-10.7456-24-24 0-13.2544 10.7456-24 24-24h208z" fill="currentColor"/></svg>',
    'desktop': '<svg class="w-4 h-4 inline-block text-gray-400" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg"><path d="M788.144 268.464h-539.52c-9.312 0-17.232 3.28-23.872 9.872-6.56 6.56-9.872 14.496-9.872 23.776v336.272c0 9.28 3.328 17.184 9.872 23.792 6.64 6.56 14.56 9.856 23.872 9.856h539.52c9.328 0 17.264-3.312 23.84-9.856a32.32 32.32 0 0 0 9.904-23.792V302.112c0-9.28-3.296-17.216-9.92-23.776a32.352 32.352 0 0 0-23.84-9.872m-539.52-67.248h539.52c27.904 0 51.76 9.856 71.52 29.568 19.76 19.728 29.616 43.456 29.616 71.312V638.4c0 27.84-9.856 51.616-29.616 71.328-19.76 19.696-43.616 29.552-71.52 29.552H552.064v67.264h101.168c9.312 0 17.264 3.28 23.84 9.872 6.608 6.56 9.904 14.496 9.904 23.776s-3.296 17.216-9.92 23.76a32.384 32.384 0 0 1-23.824 9.888h-269.76a32.48 32.48 0 0 1-23.872-9.888 32.32 32.32 0 0 1-9.872-23.76c0-9.28 3.296-17.216 9.872-23.776a32.512 32.512 0 0 1 23.872-9.872h101.152v-67.264H248.592c-27.92 0-51.776-9.856-71.52-29.552-19.76-19.712-29.648-43.488-29.648-71.328V302.112c0-27.872 9.888-51.6 29.648-71.328 19.76-19.712 43.6-29.568 71.52-29.568" fill="currentColor"/></svg>'
  }
  return icons[device.toLowerCase()] || icons['desktop']
}

// 获取设备名称
function getDeviceName(device) {
  const names = {
    'mobile': '移动',
    'tablet': '平板',
    'desktop': '桌面'
  }
  return names[device.toLowerCase()] || device
}

// 解析 content HTML 生成 navItems
function parseNavItemsFromContent(content) {
  if (!content) return []
  
  // 创建临时 DOM 解析 HTML
  const temp = document.createElement('div')
  temp.innerHTML = content
  
  // 提取所有 H2 标签
  const h2s = temp.querySelectorAll('h2')
  const items = []
  
  h2s.forEach((h2, index) => {
    const id = h2.id || `section-${index}`
    h2.id = id  // 确保有 id
    items.push({
      href: `#${id}`,
      label: h2.textContent.trim()
    })
  })
  
  return items
}

// 移动导航指示器
function moveIndicator(item) {
  if (!item || !navWrapper.value || !navIndicator.value) return
  
  const wrapRect = navWrapper.value.getBoundingClientRect()
  const itemRect = item.getBoundingClientRect()
  
  navIndicator.value.style.left = (itemRect.left - wrapRect.left + navWrapper.value.scrollLeft) + 'px'
  navIndicator.value.style.width = itemRect.width + 'px'
  
  item.scrollIntoView({ behavior: 'smooth', inline: 'center', block: 'nearest' })
}

// 更新激活的导航
function updateActiveNav() {
  const sections = document.querySelectorAll('.resource-content section[id]')
  const scrollPosition = window.scrollY + 150
  
  sections.forEach((section, index) => {
    const sectionTop = section.offsetTop
    const sectionHeight = section.offsetHeight
    
    if (scrollPosition >= sectionTop && scrollPosition < sectionTop + sectionHeight) {
      activeIndex.value = index
      const navItem = navItemRefs.value[index]
      if (navItem) {
        moveIndicator(navItem)
      }
    }
  })
}

// 滚动到指定 section
function scrollToSection(href) {
  const targetId = href.replace('#', '')
  const targetSection = document.getElementById(targetId)
  if (targetSection) {
    targetSection.scrollIntoView({ behavior: 'smooth' })
  }
}

// 构建圆角矩形路径
function buildRoundedRect(w, h, r) {
  r = Math.min(r, w / 2, h / 2)
  return 'M ' + (w / 2) + ' 0' +
    ' L ' + (w - r) + ' 0' +
    ' A ' + r + ' ' + r + ' 0 0 1 ' + w + ' ' + r +
    ' L ' + w + ' ' + (h - r) +
    ' A ' + r + ' ' + r + ' 0 0 1 ' + (w - r) + ' ' + h +
    ' L ' + r + ' ' + h +
    ' A ' + r + ' ' + r + ' 0 0 1 0 ' + (h - r) +
    ' L 0 ' + r +
    ' A ' + r + ' ' + r + ' 0 0 1 ' + r + ' 0' +
    ' Z'
}

// 更新描边进度条尺寸
function updateDimensions() {
  const svg = progressSvg.value
  const bgPath = progressBgPath.value
  const progress = progressPath.value
  const nav = document.getElementById('top-nav')
  if (!svg || !bgPath || !progress || !nav) return
  
  const w = nav.offsetWidth
  const h = nav.offsetHeight
  svg.setAttribute('viewBox', '0 0 ' + w + ' ' + h)
  const d = buildRoundedRect(w, h, 40)
  bgPath.setAttribute('d', d)
  progress.setAttribute('d', d)
  perimeter.value = progress.getTotalLength()
  bgPath.style.strokeDasharray = perimeter.value
  bgPath.style.strokeDashoffset = 0
  progress.style.strokeDasharray = perimeter.value
  progress.style.strokeDashoffset = perimeter.value
}

// 更新描边进度
function updateProgress() {
  const progress = progressPath.value
  if (!progress) return
  const scrollTop = window.scrollY
  const docHeight = document.documentElement.scrollHeight - window.innerHeight
  const progressRatio = docHeight > 0 ? Math.min(scrollTop / docHeight, 1) : 0
  progress.style.strokeDashoffset = perimeter.value * (1 - progressRatio)
}

// 初始化替代推荐卡片横滑功能
function initScrollButtons() {
  nextTick(() => {
    const scrollContainer = document.querySelector('.resource-content .cards-scroll')
    const scrollLeftBtn = document.querySelector('.resource-content .scroll-left')
    const scrollRightBtn = document.querySelector('.resource-content .scroll-right')
    
    if (!scrollContainer) return
    
    // 更新箭头显示状态
    const updateArrowVisibility = () => {
      if (scrollLeftBtn && scrollRightBtn) {
        scrollLeftBtn.style.opacity = scrollContainer.scrollLeft > 10 ? '1' : '0.3'
        scrollLeftBtn.style.pointerEvents = scrollContainer.scrollLeft > 10 ? 'auto' : 'none'
        scrollRightBtn.style.opacity = 
          scrollContainer.scrollLeft < scrollContainer.scrollWidth - scrollContainer.clientWidth - 10 
            ? '1' : '0.3'
        scrollRightBtn.style.pointerEvents = 
          scrollContainer.scrollLeft < scrollContainer.scrollWidth - scrollContainer.clientWidth - 10 
            ? 'auto' : 'none'
      }
    }
    
    // 绑定滚动事件
    scrollContainer.addEventListener('scroll', updateArrowVisibility)
    updateArrowVisibility()
    
    // 绑定左右按钮
    if (scrollLeftBtn) {
      scrollLeftBtn.addEventListener('click', () => {
        scrollContainer.scrollBy({ left: -280, behavior: 'smooth' })
      })
    }
    if (scrollRightBtn) {
      scrollRightBtn.addEventListener('click', () => {
        scrollContainer.scrollBy({ left: 280, behavior: 'smooth' })
      })
    }
  })
}

// 滚动事件处理
let scrollHandler = null
let progressScrollHandler = null
let resizeHandler = null

onMounted(async () => {
  try {
    // 加载资源数据
    const id = route.params.id
    const data = await fetchResourceById(id)
    if (data) {
      resource.value = data
      
      // 解析内容生成导航
      nextTick(() => {
        navItems.value = parseNavItemsFromContent(data.content || '')
        
        // 给实际渲染的 h2 补上 id，确保导航锚点可用
        nextTick(() => {
          const contentEl = document.querySelector('.resource-content')
          if (contentEl) {
            const h2s = contentEl.querySelectorAll('h2')
            h2s.forEach((h2, index) => {
              if (!h2.id) h2.id = `section-${index}`
            })
          }
        })
        
        // 初始化滚动监听
        scrollHandler = () => {
          requestAnimationFrame(updateActiveNav)
        }
        progressScrollHandler = () => {
          requestAnimationFrame(updateProgress)
        }
        resizeHandler = () => {
          updateDimensions()
          updateProgress()
        }
        
        window.addEventListener('scroll', scrollHandler)
        window.addEventListener('scroll', progressScrollHandler)
        window.addEventListener('resize', resizeHandler)
        
        // 初始化时设置指示器和进度条
        nextTick(() => {
          if (navItemRefs.value[0]) {
            moveIndicator(navItemRefs.value[0])
          }
          updateActiveNav()
          updateDimensions()
          updateProgress()
        })
        
        // 滚动淡入动画
        nextTick(() => {
          const fadeSections = document.querySelectorAll('.resource-content section')
          const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
              if (entry.isIntersecting) {
                entry.target.classList.add('visible')
                observer.unobserve(entry.target)
              }
            })
          }, { threshold: 0.1, rootMargin: '0px 0px -40px 0px' })
          fadeSections.forEach(section => observer.observe(section))
        })
        
        // 初始化替代推荐横滑
        initScrollButtons()
      })
    } else {
      error.value = '资源不存在'
    }
  } catch (err) {
    console.error('加载资源失败:', err)
    error.value = '加载失败，请稍后重试'
  }
})

onUnmounted(() => {
  if (scrollHandler) {
    window.removeEventListener('scroll', scrollHandler)
  }
  if (progressScrollHandler) {
    window.removeEventListener('scroll', progressScrollHandler)
  }
  if (resizeHandler) {
    window.removeEventListener('resize', resizeHandler)
  }
})
</script>

<style scoped>
/* 全局滚动行为 */
html {
  scroll-behavior: smooth;
}

/* 页面容器透明，让全局几何背景可见 */
.resource-container {
  background: transparent;
}

/* 卡片容器 */
.detail-card {
  background-color: #ffffff;
  border-radius: 25px;
  box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.06), 0 1px 2px -1px rgba(0, 0, 0, 0.06);
  padding: 0 0 1.5rem 0;
  overflow: hidden;
}

/* 暗色模式卡片 */
:global(.dark) .detail-card,
:global(html.dark) .detail-card,
:global([data-theme="dark"]) .detail-card {
  background-color: #1e1e2e;
  box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.2);
}

/* 导航项样式 */
:deep(.nav-item) {
  transition: color 0.2s ease;
}

:deep(.nav-item:hover) {
  color: #22c55e;
}

:deep(.nav-item.active) {
  color: #22c55e;
}

/* 导航指示器 */
:deep(.nav-indicator) {
  position: absolute;
  bottom: 0;
  height: 2px;
  background: #86efac;
  border-radius: 2px;
  transition: left 0.3s ease, width 0.3s ease;
}

/* 资源内容样式 */
:deep(.resource-content) {
  max-width: 100%;
}

:deep(.resource-content h2) {
  font-size: 1.25rem;
  font-weight: 700;
  color: #111827;
  margin-bottom: 0.75rem;
  padding-top: 4rem;
  margin-top: -4rem;
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

:deep(.resource-content h3) {
  font-size: 1.0625rem;
  font-weight: 600;
  color: #374151;
  margin-bottom: 0.5rem;
  margin-top: 1rem;
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 0.375rem;
}

:deep(.resource-content p) {
  color: #374151;
  line-height: 1.75;
  font-size: 1rem;
  margin-bottom: 0.75rem;
  text-indent: 2em;
}

:deep(.resource-content section) {
  margin-bottom: 2rem;
  padding-bottom: 2rem;
  border-bottom: 1px solid #f3f4f6;
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.6s ease, transform 0.6s ease;
}

:deep(.resource-content section:last-child) {
  border-bottom: none;
}

:deep(.resource-content section.visible) {
  opacity: 1;
  transform: translateY(0);
}

/* 段落样式 */
:deep(.paragraph) {
  text-indent: 2em;
  margin-bottom: 1.25rem;
  color: #374151;
  line-height: 1.75;
}

/* 图片占位容器 */
:deep(.img-placeholder) {
  margin: 1.5rem 0;
  width: 100%;
  aspect-ratio: 16 / 9;
  background-color: #e5e7eb;
  border-radius: 25px;
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
  position: relative;
  transition: transform 0.2s ease;
}

:deep(.img-placeholder:hover) {
  transform: scale(1.02);
}

:deep(.img-placeholder .placeholder-icon) {
  width: 50%;
  height: 50%;
  opacity: 0.3;
  transition: opacity 0.3s ease;
}

:deep(.img-placeholder .actual-image) {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  object-fit: cover;
  opacity: 0;
  transition: opacity 0.3s ease;
  border-radius: 25px;
}

:deep(.img-placeholder.img-loaded .actual-image) {
  opacity: 1;
}

:deep(.img-placeholder.img-loaded .placeholder-icon) {
  opacity: 0;
}

/* 截图预览网格 */
:deep(.screenshots-grid) {
  display: grid;
  grid-template-columns: repeat(1, 1fr);
  gap: 1rem;
}

@media (min-width: 640px) {
  :deep(.screenshots-grid) {
    grid-template-columns: repeat(2, 1fr);
  }
}

@media (min-width: 1024px) {
  :deep(.screenshots-grid) {
    grid-template-columns: repeat(3, 1fr);
  }
}

:deep(.screenshots-grid .img-placeholder) {
  margin: 0;
  border-radius: 20px;
  aspect-ratio: 16 / 9;
}

:deep(.screenshots-grid .img-placeholder .placeholder-icon) {
  width: 33%;
  height: 33%;
}

/* 优缺点对比面板 */
:deep(.proscons-wrapper) {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

@media (min-width: 1024px) {
  :deep(.proscons-wrapper) {
    flex-direction: row;
  }
}

:deep(.pros-panel) {
  flex: 1;
  background-color: #f0fdf4;
  border-radius: 25px;
  padding: 1.25rem;
}

:deep(.pros-panel h3) {
  font-size: 1.125rem;
  font-weight: 700;
  color: #15803d;
  margin-bottom: 1rem;
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

:deep(.cons-panel) {
  flex: 1;
  background-color: #fef2f2;
  border-radius: 25px;
  padding: 1.25rem;
}

:deep(.cons-panel h3) {
  font-size: 1.125rem;
  font-weight: 700;
  color: #b91c1c;
  margin-bottom: 1rem;
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

:deep(.pros-panel ul),
:deep(.cons-panel ul) {
  list-style: none;
  padding: 0;
  margin: 0;
}

:deep(.pros-panel li),
:deep(.cons-panel li) {
  display: flex;
  align-items: flex-start;
  gap: 0.5rem;
  color: #374151;
  line-height: 1.75;
  margin-bottom: 0.75rem;
}

:deep(.pros-panel li .check) {
  color: #22c55e;
  margin-top: 0.125rem;
  flex-shrink: 0;
}

:deep(.cons-panel li .cross) {
  color: #ef4444;
  margin-top: 0.125rem;
  flex-shrink: 0;
}

/* 替代推荐卡片 */
:deep(.alternatives-wrapper) {
  position: relative;
}

:deep(.scroll-btn) {
  position: absolute;
  top: 50%;
  transform: translateY(-50%);
  z-index: 10;
  width: 2.5rem;
  height: 2.5rem;
  border-radius: 50%;
  background-color: rgba(255, 255, 255, 0.9);
  box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1);
  display: none;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  transition: all 0.2s ease;
  border: none;
}

@media (min-width: 640px) {
  :deep(.scroll-btn) {
    display: flex;
  }
}

:deep(.scroll-btn:hover) {
  background-color: white;
  box-shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.1);
}

:deep(.scroll-left) {
  left: -3.5rem;
}

:deep(.scroll-right) {
  right: -3.5rem;
}

:deep(.cards-scroll) {
  display: flex;
  gap: 1rem;
  overflow-x: auto;
  padding-bottom: 1rem;
  scroll-snap-type: x mandatory;
  -webkit-overflow-scrolling: touch;
}

:deep(.cards-scroll > *) {
  scroll-snap-align: start;
}

:deep(.alt-card) {
  flex-shrink: 0;
  width: 16rem;
  background-color: white;
  border-radius: 20px;
  box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.1);
  overflow: hidden;
  display: block;
  text-decoration: none;
  color: inherit;
  transition: transform 0.2s ease, box-shadow 0.2s ease;
}

:deep(.alt-card:hover) {
  transform: scale(1.02);
  box-shadow: 0 8px 25px -5px rgba(0, 0, 0, 0.1), 0 4px 10px -6px rgba(0, 0, 0, 0.1);
}

:deep(.alt-card .img-placeholder) {
  margin: 0;
  aspect-ratio: 16 / 9;
  border-radius: 0;
}

:deep(.alt-card .img-placeholder .placeholder-icon) {
  width: 50%;
  height: 50%;
}

:deep(.alt-card-info) {
  padding: 1rem;
}

:deep(.alt-card-info h4) {
  font-weight: 700;
  color: #111827;
  margin-bottom: 0.25rem;
}

:deep(.alt-card-info p) {
  font-size: 0.875rem;
  color: #6b7280;
  margin-bottom: 0.75rem;
  text-indent: 0;
}

:deep(.alt-tags) {
  display: flex;
  gap: 0.5rem;
}

:deep(.tag) {
  background-color: transparent;
  color: #6b7280;
  border-radius: 20px;
  font-size: 0.75rem;
  padding: 0.25rem 0.625rem;
}

:deep(.tag.text-orange-600) {
  color: #ea580c;
}

:deep(.tag.text-green-600) {
  color: #16a34a;
}

/* 隐藏滚动条 */
:deep(.hide-scrollbar) {
  -ms-overflow-style: none;
  scrollbar-width: none;
}

:deep(.hide-scrollbar::-webkit-scrollbar) {
  display: none;
}

/* 减少动画偏好 */
@media (prefers-reduced-motion: reduce) {
  :deep(.resource-content section) {
    opacity: 1;
    transform: none;
    transition: none;
  }
  :deep(.alt-card) {
    transition: none;
  }
  :deep(.alt-card:hover) {
    transform: none;
  }
}
</style>
