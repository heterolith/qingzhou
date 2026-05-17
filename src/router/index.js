import { createRouter, createWebHistory } from 'vue-router'

// 路由配置
const routes = [
  {
    path: '/',
    name: 'home',
    component: () => import('../views/HomeView.vue'),
    meta: { title: '晴宙' }
  },
  {
    path: '/works',
    name: 'works',
    component: () => import('../views/WorksView.vue'),
    meta: { title: '文章展示 | 晴宙' }
  },
  {
    path: '/works/:id',
    name: 'work-detail',
    component: () => import('../views/WorkDetailView.vue'),
    meta: { title: '作品详情 | 晴宙' }
  },
  {
    path: '/about',
    name: 'about',
    component: () => import('../views/AboutView.vue'),
    meta: { title: '关于 - 晴宙博客' }
  },
  {
    path: '/resources',
    name: 'resources',
    component: () => import('../views/ResourcesView.vue'),
    meta: { title: '资源分享 | 晴宙' }
  },
  {
    path: '/resources/:id',
    name: 'resource-detail',
    component: () => import('../views/ResourceDetailView.vue'),
    meta: { title: '资源详情 | 晴宙' }
  },
  {
    path: '/friends',
    name: 'friends',
    component: () => import('../views/FriendsView.vue'),
    meta: { title: '友情链接 | 晴宙' }
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes,
  scrollBehavior(to, from, savedPosition) {
    // 刷新时总是回到顶部，忽略URL中的锚点
    if (savedPosition) {
      return { top: 0 }
    }
    // 正常导航时也回到顶部
    return { top: 0 }
  }
})

// 路由守卫 - 更新页面标题并清除URL中的锚点
router.beforeEach((to, from, next) => {
  document.title = to.meta.title || '晴宙'
  
  // 清除URL中的hash锚点，防止刷新时自动跳转
  if (window.location.hash && to.path === '/') {
    window.history.replaceState(null, '', window.location.pathname)
  }
  
  next()
})

export default router
