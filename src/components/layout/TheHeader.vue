<template>
  <header class="header" :class="{ scrolled: isScrolled }">
    <router-link to="/" class="logo">
      晴宙<span class="logo-dot"></span>
    </router-link>
    
    <!-- 桌面端导航：切换按钮在导航选项左侧 -->
    <nav class="nav-links">
      <div class="nav-item theme-btn">
        <IconTheme :preference="themePreference" @toggle="handleThemeToggle" />
      </div>
      <a 
        v-for="link in navLinks" 
        :key="link.name"
        href="javascript:void(0)"
        class="nav-item nav-link"
        :class="{ active: isActiveLink(link) }"
        @click="handleNavClick(link)"
      >
        <IconHome v-if="link.icon === 'home'" />
        <IconArticle v-else-if="link.icon === 'article'" />
        <IconResource v-else-if="link.icon === 'resource'" />
        <IconAbout v-else-if="link.icon === 'about'" />
        <IconContact v-else-if="link.icon === 'contact'" />
        <IconLink v-else-if="link.icon === 'link'" />
        <span>{{ link.name }}</span>
      </a>
    </nav>
    
    <!-- 移动端：切换按钮在左，汉堡菜单在右 -->
    <div class="mobile-actions">
      <IconTheme :preference="themePreference" @toggle="handleThemeToggle" />
      <div class="mobile-toggle" @click="$emit('toggle-sidebar')">
        <IconHamburger :is-active="isSidebarOpen" />
      </div>
    </div>
  </header>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import IconHamburger from '../icons/IconHamburger.vue'
import IconTheme from '../icons/IconTheme.vue'
import IconHome from '../icons/IconHome.vue'
import IconArticle from '../icons/IconArticle.vue'
import IconResource from '../icons/IconResource.vue'
import IconAbout from '../icons/IconAbout.vue'
import IconContact from '../icons/IconContact.vue'
import IconLink from '../icons/IconLink.vue'
import { getThemeState, toggleTheme } from '../../utils/theme.js'

const route = useRoute()
const router = useRouter()

// 主题状态
const themePreference = ref('auto')

// 导航链接
const navLinks = [
  { name: '首页', path: '/', icon: 'home' },
  { name: '文章', path: '/works', icon: 'article' },
  { name: '资源', path: '/resources', icon: 'resource' },
  { name: '关于', path: '/about', icon: 'about' },
  { name: '联系', path: '/', hash: 'contact', icon: 'contact' },
  { name: '友链', path: '/friends', icon: 'link' }
]

// 滚动状态
const isScrolled = ref(false)

// 侧边栏状态
defineProps({
  isSidebarOpen: {
    type: Boolean,
    default: false
  }
})

// 判断链接是否激活
const isActiveLink = (link) => {
  if (link.hash) {
    return false
  }
  return route.path === link.path
}

// 处理导航点击
const handleNavClick = (link) => {
  if (link.hash) {
    if (route.path !== '/') {
      router.push('/')
      setTimeout(() => {
        scrollToSection(link.hash)
      }, 100)
    } else {
      scrollToSection(link.hash)
    }
  } else {
    if (route.path !== link.path) {
      router.push(link.path)
    }
  }
}

// 滚动到指定区域
const scrollToSection = (sectionId) => {
  const element = document.getElementById(sectionId)
  if (element) {
    const headerHeight = 110
    const targetPosition = element.offsetTop - headerHeight
    window.scrollTo({
      top: targetPosition,
      behavior: 'smooth'
    })
  }
}

// 滚动监听
const handleScroll = () => {
  isScrolled.value = window.scrollY > 50
}

// 主题切换
const handleThemeToggle = () => {
  const state = toggleTheme()
  themePreference.value = state.preference
}

onMounted(() => {
  window.addEventListener('scroll', handleScroll)
  const state = getThemeState()
  themePreference.value = state.preference
})

onUnmounted(() => {
  window.removeEventListener('scroll', handleScroll)
})

defineEmits(['toggle-sidebar'])
</script>

<style scoped>
.header {
  position: fixed;
  top: var(--notice-height);
  left: 0;
  right: 0;
  height: var(--header-height);
  background: rgba(255, 255, 255, 0.95);
  box-shadow: var(--shadow-sm);
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 5%;
  z-index: 999;
  transition: all var(--transition-normal);
}

.header.scrolled {
  background: rgba(255, 255, 255, 0.95);
  box-shadow: var(--shadow-sm);
}

.logo {
  font-size: 1.7rem;
  font-weight: 600;
  letter-spacing: -1px;
  text-decoration: none;
  color: var(--primary);
  display: flex;
  align-items: center;
  flex-shrink: 0;
}

.logo-dot {
  width: 8px;
  height: 8px;
  background: var(--accent);
  border-radius: 50%;
  margin-left: 4px;
  margin-top: 4px;
  transition: var(--transition);
}

/* 导航链接容器 - 右侧 */
.nav-links {
  display: flex;
  align-items: center;
  gap: 1.5rem;
}

.nav-item {
  display: flex;
  align-items: center;
}

.nav-link {
  font-size: 0.9rem;
  font-weight: 500;
  color: var(--text-secondary);
  position: relative;
  padding: 0.5rem 0;
  display: flex;
  align-items: center;
  gap: 6px;
}

.nav-link::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 0;
  width: 0;
  height: 2px;
  background: var(--accent);
  transition: width var(--transition-fast);
}

.nav-link:hover::after,
.nav-link.active::after {
  width: 100%;
}

.nav-link.active {
  color: var(--accent);
}

/* 移动端操作区 - 右侧 */
.mobile-actions {
  display: none;
  flex-direction: row;
  align-items: center;
  gap: 8px;
}

.mobile-toggle {
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
}

@media (max-width: 768px) {
  .nav-links {
    display: none;
  }

  .mobile-actions {
    display: flex;
    flex-direction: row;
  }
}
</style>
