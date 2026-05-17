<template>
  <!-- 遮罩层 -->
  <div 
    class="sidebar-overlay" 
    :class="{ active: visible }"
    @click="$emit('close')"
  ></div>
  
  <!-- 侧边栏 -->
  <nav class="sidebar-nav" :class="{ active: visible }">
    <div class="sidebar-header">
      <!-- 左上角：主题切换按钮 -->
      <div class="sidebar-theme-toggle">
        <IconTheme :preference="themePreference" @toggle="handleThemeToggle" />
      </div>
      
      <!-- 右上角：关闭按钮 -->
      <button class="sidebar-close" @click="$emit('close')">
        <span class="close-line close-line-1"></span>
        <span class="close-line close-line-2"></span>
      </button>
      
      <img :src="siteInfo.avatar" :alt="siteInfo.name + '的头像'" class="sidebar-avatar" @error="onAvatarError">
      <h3 class="sidebar-name">{{ siteInfo.name }}</h3>
    </div>
    <div class="sidebar-content">
      <ul class="sidebar-list">
        <li class="sidebar-item" v-for="item in navItems" :key="item.name">
          <a 
            href="javascript:void(0)" 
            class="sidebar-link" 
            :class="{ active: isActiveLink(item) }" 
            @click="handleNavClick(item)"
          >
            <IconHome v-if="item.icon === 'home'" />
            <IconArticle v-else-if="item.icon === 'article'" />
            <IconResource v-else-if="item.icon === 'resource'" />
            <IconAbout v-else-if="item.icon === 'about'" />
            <IconContact v-else-if="item.icon === 'contact'" />
            <IconLink v-else-if="item.icon === 'link'" />
            <span>{{ item.name }}</span>
          </a>
        </li>
      </ul>
    </div>
    <div class="sidebar-footer">
      <p>&copy; {{ new Date().getFullYear() }} {{ siteInfo.name }}</p>
    </div>
  </nav>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import IconTheme from '../icons/IconTheme.vue'
import IconHome from '../icons/IconHome.vue'
import IconArticle from '../icons/IconArticle.vue'
import IconResource from '../icons/IconResource.vue'
import IconAbout from '../icons/IconAbout.vue'
import IconContact from '../icons/IconContact.vue'
import IconLink from '../icons/IconLink.vue'
import { getThemeState, toggleTheme } from '../../utils/theme.js'
import { storageUrl } from '../../lib/supabase.js'

const route = useRoute()
const router = useRouter()

// 主题状态
const themePreference = ref('auto')

// 网站信息数据
const siteInfo = {
  name: '晴宙',
  avatar: `${storageUrl}/avatar/avatar.jpg`
}

// 导航菜单数据
const navItems = [
  { name: '首页', path: '/', icon: 'home' },
  { name: '文章', path: '/works', icon: 'article' },
  { name: '资源', path: '/resources', icon: 'resource' },
  { name: '关于', path: '/about', icon: 'about' },
  { name: '联系', path: '/', hash: 'contact', icon: 'contact' },
  { name: '友链', path: '/friends', icon: 'link' }
]

// 判断链接是否激活
const isActiveLink = (item) => {
  if (item.hash) {
    return false
  }
  return route.path === item.path
}

// 处理导航点击
const handleNavClick = (item) => {
  emit('close')
  
  if (item.hash) {
    if (route.path !== item.path) {
      router.push(item.path)
      setTimeout(() => {
        scrollToSection(item.hash)
      }, 100)
    } else {
      scrollToSection(item.hash)
    }
  } else {
    if (route.path !== item.path) {
      router.push(item.path)
    } else {
      window.scrollTo({
        top: 0,
        behavior: 'smooth'
      })
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

// 头像加载失败处理
const onAvatarError = (e) => {
  e.target.style.backgroundColor = '#cccccc'
}

// 主题切换
const handleThemeToggle = () => {
  const state = toggleTheme()
  themePreference.value = state.preference
}

defineProps({
  visible: {
    type: Boolean,
    default: false
  }
})

const emit = defineEmits(['close'])

onMounted(() => {
  const state = getThemeState()
  themePreference.value = state.preference
})
</script>

<style scoped>
.sidebar-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  z-index: 9998;
  opacity: 0;
  visibility: hidden;
  transition: all 0.3s ease;
}

.sidebar-overlay.active {
  opacity: 1;
  visibility: visible;
}

.sidebar-nav {
  position: fixed;
  top: 0;
  right: -320px;
  width: 300px;
  height: 100vh;
  background-color: var(--bg-primary);
  z-index: 9999;
  transition: right 0.4s cubic-bezier(0.165, 0.84, 0.44, 1);
  box-shadow: -8px 0 30px rgba(0, 0, 0, 0.12);
  display: flex;
  flex-direction: column;
}

.sidebar-nav.active {
  right: 0;
}

.sidebar-header {
  padding: 50px 20px 20px;
  background: var(--bg-primary);
  color: var(--primary);
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
  position: relative;
  flex-shrink: 0;
}

.sidebar-theme-toggle {
  position: absolute;
  top: 18px;
  left: 18px;
  z-index: 10;
}

.sidebar-close {
  position: absolute;
  top: 18px;
  right: 18px;
  background: transparent;
  border: none;
  cursor: pointer;
  transition: all 0.3s ease;
  width: 36px;
  height: 36px;
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 10;
}

.close-line {
  position: absolute;
  width: 20px;
  height: 2px;
  background: var(--primary);
  transition: all 0.3s ease;
}

.close-line-1 {
  transform: rotate(45deg);
}

.close-line-2 {
  transform: rotate(-45deg);
}

.sidebar-close:hover .close-line {
  background: #5e81f4;
}

.sidebar-avatar {
  width: 115px;
  height: 115px;
  border-radius: 50%;
  border: 5px solid #5e81f4;
  box-shadow: 0 0 20px rgba(94, 129, 244, 0.35);
  transition: transform 0.3s ease;
  object-fit: cover;
}

.sidebar-avatar:hover {
  transform: scale(1.05);
}

.sidebar-name {
  margin: 0;
  font-size: 1.1rem;
  color: var(--primary);
  font-weight: 600;
}

.sidebar-content {
  flex: 1;
  position: relative;
  padding: 0;
}

.sidebar-list {
  list-style: none;
  display: flex;
  flex-direction: column;
  gap: 20px;
  width: 100%;
  position: absolute;
  top: 35%;
  left: 0;
  transform: translateY(-50%);
  margin: 0;
  padding: 0;
}

.sidebar-item {
  position: relative;
  margin: 0;
  padding: 0 35px;
  display: flex;
  justify-content: center;
}

.sidebar-item::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 25px;
  right: 25px;
  height: 1px;
  background: var(--border-color);
  opacity: 0.5;
}

.sidebar-item:last-child::after {
  display: none;
}

.sidebar-link {
  display: flex;
  align-items: center;
  gap: 15px;
  padding: 16px 25px;
  text-decoration: none;
  color: var(--text-primary);
  font-size: 1rem;
  font-weight: 500;
  border-radius: 0;
  transition: all 0.3s ease;
  position: relative;
}

.sidebar-link::before {
  content: '';
  position: absolute;
  left: 0;
  top: 50%;
  transform: translateY(-50%);
  width: 4px;
  height: 0;
  background: #5e81f4;
  transition: height 0.3s ease;
  border-radius: 0 2px 2px 0;
}

.sidebar-link:hover::before {
  height: 60%;
}

.sidebar-link:hover {
  background-color: rgba(94, 129, 244, 0.08);
  color: #5e81f4;
  transform: translateX(5px);
}

.sidebar-link.active {
  color: #5e81f4;
  background-color: rgba(94, 129, 244, 0.12);
  font-weight: 600;
}

.sidebar-link.active::before {
  height: 70%;
}

.sidebar-link i,
.sidebar-link svg {
  font-size: 1.15rem;
  color: #5e81f4;
  width: 20px;
  height: 20px;
  flex-shrink: 0;
  text-align: center;
  transition: all 0.3s ease;
}

.sidebar-link:hover i,
.sidebar-link:hover svg {
  transform: scale(1.1);
}

.sidebar-footer {
  padding: 15px 25px;
  border-top: 1px solid var(--border-color);
  display: flex;
  justify-content: center;
  flex-shrink: 0;
}

.sidebar-footer p {
  margin: 0;
  font-size: 0.85rem;
  color: var(--text-muted);
}
</style>
