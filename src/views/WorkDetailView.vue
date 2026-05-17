<template>
  <div class="article-container" v-if="work">
    <!-- 返回按钮 -->
    <div class="max-w-4xl mx-auto px-4 sm:px-6 pt-6">
      <router-link to="/works" class="inline-flex items-center text-gray-500 hover:text-blue-500 transition-colors">
        <svg class="w-5 h-5 mr-1" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
          <path d="M256 120l-45.25 45.25L458.75 512 210.75 758.75 256 804l320-320z"/>
        </svg>
        返回列表
      </router-link>
    </div>
    
    <!-- 卡片容器 -->
    <div class="detail-card max-w-4xl mx-auto mt-6 mb-10">
      <!-- 主图 -->
      <div v-if="work.image">
        <div class="w-full aspect-video bg-gray-200 overflow-hidden rounded-t-[25px]">
          <img :src="work.image" :alt="work.title" class="w-full h-full object-cover">
        </div>
      </div>
      
      <!-- 基础信息 -->
      <header class="px-4 sm:px-8 pt-2 pb-6 border-b border-gray-100">
        <h1 class="text-2xl sm:text-3xl font-bold text-gray-900 mb-6 text-center">{{ work.title }}</h1>
        <div class="flex justify-center gap-8 text-sm text-gray-500 mb-5 flex-wrap">
          <p class="flex items-center gap-1.5">
            <svg class="w-4 h-4 text-orange-500" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
              <path d="M512 1004.307692C240.443077 1004.307692 19.692308 783.556923 19.692308 512 19.692308 240.64 240.443077 19.692308 512 19.692308s492.307692 220.750769 492.307692 492.307692-220.750769 492.307692-492.307692 492.307692z m0-945.230769C262.301538 59.076923 59.076923 262.301538 59.076923 512s203.224615 452.923077 452.923077 452.923077 452.923077-203.224615 452.923077-452.923077S761.698462 59.076923 512 59.076923z"/>
              <path d="M512 609.870769c-108.504615 0-196.923077-88.418462-196.923077-196.923077s88.418462-196.923077 196.923077-196.923077 196.923077 88.418462 196.923077 196.923077-88.418462 196.923077-196.923077 196.923077z m0-354.461538c-86.843077 0-157.538462 70.695385-157.538462 157.538461s70.695385 157.538462 157.538462 157.538462 157.538462-70.695385 157.538462-157.538462-70.695385-157.538462-157.538462-157.538461z"/>
              <path d="M173.095385 859.963077c-1.969231 0-3.938462-0.196923-5.71077-0.787692a19.633231 19.633231 0 0 1-12.996923-24.615385c48.64-157.932308 192.393846-263.876923 357.612308-263.876923s308.972308 105.944615 357.612308 263.876923a19.633231 19.633231 0 0 1-12.996923 24.615385 19.633231 19.633231 0 0 1-24.615385-12.996923c-43.52-141.587692-172.110769-236.307692-320-236.307693S235.52 704.787692 192 845.981538c-2.56 8.467692-10.436923 13.981538-18.904615 13.981539z"/>
            </svg>
            {{ work.author || '晴宙' }}
          </p>
          <p class="flex items-center gap-1.5">
            <svg class="w-4 h-4 text-slate-400" viewBox="0 0 1024 1024" xmlns="http://www.w3.org/2000/svg" fill="currentColor">
              <path d="M802.4576 896H233.472A115.6608 115.6608 0 0 1 117.76 780.4928V318.976a115.6608 115.6608 0 0 1 115.712-115.5072h41.8304a30.72 30.72 0 0 1 0 61.44H233.472A54.1696 54.1696 0 0 0 179.2 318.976v461.4656A54.1696 54.1696 0 0 0 233.472 834.56h568.9856a54.1696 54.1696 0 0 0 54.0672-54.0672V318.976a54.1696 54.1696 0 0 0-54.0672-54.0672h-38.7072a30.72 30.72 0 0 1 0-61.44h38.7072A115.6608 115.6608 0 0 1 918.016 318.976v461.4656A115.6608 115.6608 0 0 1 802.4576 896z"/>
              <path d="M610.7136 264.9088h-189.44a30.72 30.72 0 0 1 0-61.44h189.44a30.72 30.72 0 1 1 0 61.44zM877.9264 482.304H158.0032a30.72 30.72 0 0 1 0-61.44h719.9232a30.72 30.72 0 0 1 0 61.44zM347.3408 346.7264a30.72 30.72 0 0 1-30.72-30.72V166.4a30.72 30.72 0 0 1 61.44 0v149.6064a30.72 30.72 0 0 1-30.72 30.72zM688.5888 346.7264a30.72 30.72 0 0 1-30.72-30.72V166.4a30.72 30.72 0 0 1 61.44 0v149.6064a30.72 30.72 0 0 1-30.72 30.72z"/>
            </svg>
            {{ formatDate(work.time) }}
          </p>
          <span class="px-2.5 py-0.5 rounded-[20px] text-xs bg-gray-200 text-gray-800">AI辅助生成</span>
        </div>
        <div class="flex justify-center gap-2 mt-4 flex-wrap">
          <span 
            v-for="cat in categories" 
            :key="cat" 
            class="px-2.5 py-0.5 border rounded-[40px] text-sm text-gray-700"
            :style="{ borderColor: getCategoryColor(cat) }"
          >{{ cat }}</span>
        </div>
      </header>
      
      <!-- 正文内容 -->
      <div class="px-4 sm:px-8 pt-6 pb-10">
        <div class="article-content" v-html="work.content"></div>
        
        <!-- 底部 -->
        <footer class="pt-6 border-t border-gray-200 text-sm text-gray-500 text-center mt-8">
          <p>© 2026 {{ work.author || '晴宙' }} · 保留全部著作权利</p>
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
import { fetchWorkById } from '../utils/dataLoader.js'

const route = useRoute()
const work = ref(null)
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
  if (!work.value?.category) return []
  return Array.isArray(work.value.category) 
    ? work.value.category 
    : work.value.category.split(',').map(c => c.trim()).filter(Boolean)
})

// 获取分类颜色
function getCategoryColor(category) {
  const index = categories.value.indexOf(category)
  return categoryColors[index % categoryColors.length]
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

// 格式化日期
function formatDate(dateStr) {
  if (!dateStr) return ''
  const date = new Date(dateStr)
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')
  return `${year}年${month}月${day}日`
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
  const sections = document.querySelectorAll('section[id]')
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

// 滚动事件处理
let scrollHandler = null
let progressScrollHandler = null
let resizeHandler = null

onMounted(async () => {
  try {
    // 加载文章数据
    const id = route.params.id
    const data = await fetchWorkById(id)
    if (data) {
      work.value = data
      
      // 解析内容生成导航
      nextTick(() => {
        navItems.value = parseNavItemsFromContent(data.content || '')
        
        // 给实际渲染的 h2 补上 id，确保导航锚点可用
        nextTick(() => {
          const contentEl = document.querySelector('.article-content')
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
          const fadeSections = document.querySelectorAll('.article-content section')
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
      })
    } else {
      error.value = '文章不存在'
    }
  } catch (err) {
    console.error('加载文章失败:', err)
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
.article-container {
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

/* 文章内容样式 */
:deep(.article-content) {
  max-width: 720px;
  margin: 0 auto;
}

:deep(.article-content h2) {
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

:deep(.article-content h3) {
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

:deep(.article-content p) {
  color: #374151;
  line-height: 1.75;
  font-size: 1rem;
  margin-bottom: 0.75rem;
  text-indent: 2em;
}

:deep(.article-content section) {
  margin-bottom: 2rem;
  padding-bottom: 2rem;
  border-bottom: 1px solid #f3f4f6;
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.6s ease, transform 0.6s ease;
}

:deep(.article-content section:last-child) {
  border-bottom: none;
}

:deep(.article-content section.visible) {
  opacity: 1;
  transform: translateY(0);
}

:deep(.article-content ol) {
  list-style: decimal;
  list-style-position: inside;
  margin-bottom: 1.25rem;
  padding-left: 1rem;
}

:deep(.article-content li) {
  color: #374151;
  line-height: 1.75;
  font-size: 1rem;
  margin-bottom: 0.5rem;
}

/* 图片占位容器 */
:deep(.article-content .img-placeholder) {
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
}

:deep(.article-content .img-placeholder .placeholder-icon) {
  width: 50%;
  height: 50%;
  opacity: 0.3;
  transition: opacity 0.3s ease;
}

:deep(.article-content .img-placeholder .actual-image) {
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

:deep(.article-content .img-placeholder.img-loaded .actual-image) {
  opacity: 1;
}

:deep(.article-content .img-placeholder.img-loaded .placeholder-icon) {
  opacity: 0;
}

/* 红色高亮文字 */
:deep(.article-content span[style*="color"]) {
  font-weight: 500;
}

:deep(.hide-scrollbar) {
  -ms-overflow-style: none;
  scrollbar-width: none;
}

:deep(.hide-scrollbar::-webkit-scrollbar) {
  display: none;
}
</style>
