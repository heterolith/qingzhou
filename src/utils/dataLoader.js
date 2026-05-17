/**
 * 数据预加载管理器
 * 负责并行加载所有 Supabase 数据，支持缓存和静默更新
 */

import { reactive, computed } from 'vue'
import { supabase, storageUrl } from '../lib/supabase.js'
import { setCache, getCache, CACHE_KEYS, DEFAULT_CACHE_DURATION } from './dataCache.js'

// 数据加载状态 - 使用 reactive 保持 Vue 响应式
export const loadingState = reactive({
  isLoading: true,           // 是否正在首次加载
  isSilentUpdating: false,    // 是否正在静默更新
  works: [],                  // 作品数据
  resources: [],              // 资源数据
  carouselCards: [],          // 轮播卡片数据
  friends: []                 // 友链数据
})

// 回调函数列表
const callbacks = {
  onInitialLoad: null,
  onSilentUpdate: null,
  onError: null
}

/**
 * 注册回调函数
 */
export function registerCallbacks({ onInitialLoad, onSilentUpdate, onError }) {
  if (onInitialLoad) callbacks.onInitialLoad = onInitialLoad
  if (onSilentUpdate) callbacks.onSilentUpdate = onSilentUpdate
  if (onError) callbacks.onError = onError
}

/**
 * 从 Supabase 获取作品数据
 */
async function fetchWorksFromSupabase() {
  try {
    const { data, error } = await supabase
      .from('works')
      .select('*')
      .order('created_at', { ascending: false })

    if (error) {
      console.error('获取作品数据失败:', error)
      return null
    }

    // 转换数据格式
    return data.map(item => ({
      id: item.id,
      title: item.title || '',
      description: item.description || '',
      author: item.author || '本人',
      time: formatDate(item.created_at),
      category: item.category ? item.category.split(',') : [],
      image: item.image || `${storageUrl}/images/works/workes.jpg`
    }))
  } catch (err) {
    console.error('获取作品数据异常:', err)
    return null
  }
}

/**
 * 从 Supabase 获取资源数据
 */
async function fetchResourcesFromSupabase() {
  try {
    const { data, error } = await supabase
      .from('resources')
      .select('*')
      .order('created_at', { ascending: false })

    if (error) {
      console.error('获取资源数据失败:', error)
      return null
    }

    // 转换数据格式
    return data.map(item => ({
      id: item.id,
      title: item.title || '',
      link: item.link || '',
      description: item.description || '',
      recommend: getRecommendText(item.recommend || 1),
      tags: item.tags ? item.tags.split(',') : [],
      time: formatDate(item.created_at),
      category: item.category ? item.category.split(',') : [],
      icon: item.image || `${storageUrl}/images/resources/unsplash.png`
    }))
  } catch (err) {
    console.error('获取资源数据异常:', err)
    return null
  }
}

/**
 * 从 Supabase 获取友链数据
 */
async function fetchFriendsFromSupabase() {
  try {
    const { data, error } = await supabase
      .from('friends')
      .select('*')
      .order('created_at', { ascending: true })

    if (error) {
      console.error('获取友链数据失败:', error)
      return null
    }

    return data.map(item => ({
      id: item.id,
      name: item.name || '',
      desc: item.description || '',
      avatar: item.avatar || '',
      url: item.url || '#'
    }))
  } catch (err) {
    console.error('获取友链数据异常:', err)
    return null
  }
}

/**
 * 获取轮播卡片数据（从 Supabase 或本地）
 */
async function fetchCarouselFromSupabase() {
  try {
    // 尝试从 Supabase 获取
    const { data, error } = await supabase
      .from('works')
      .select('*')
      .order('created_at', { ascending: false })
      .limit(3)

    if (error || !data || data.length === 0) {
      // 如果获取失败，返回本地默认数据
      return getLocalCarouselCards()
    }

    // 转换数据格式
    return data.slice(0, 3).map((item, index) => ({
      tag: ['实战教程', '开源项目', '技术分享'][index] || '精选',
      tagBg: ['#f0f7ff', '#f0fdf4', '#fef3c7'][index] || '#f0f7ff',
      tagColor: ['#0066cc', '#16a34a', '#d97706'][index] || '#0066cc',
      title: item.title || '',
      subtitle: item.description || '',
      image: item.image || `${storageUrl}/carousel/slide${index + 1}.jpg`
    }))
  } catch (err) {
    console.error('获取轮播数据异常:', err)
    return getLocalCarouselCards()
  }
}

/**
 * 获取本地默认轮播卡片
 */
function getLocalCarouselCards() {
  return [
    {
      tag: '实战教程',
      tagBg: '#f0f7ff',
      tagColor: '#0066cc',
      title: '如何用3天从零搭建个人博客',
      subtitle: '从域名注册到服务器部署，手把手教你打造一个高性能的个人网站，包含完整的SEO优化指南和性能优化技巧。',
      image: `${storageUrl}/carousel/slide1.jpg`
    },
    {
      tag: '开源项目',
      tagBg: '#f0fdf4',
      tagColor: '#16a34a',
      title: 'React 组件库实战开发',
      subtitle: '从设计系统到组件封装，完整的企业级组件库开发指南，提升前端开发效率，打造可复用的组件体系。',
      image: `${storageUrl}/carousel/slide2.jpg`
    },
    {
      tag: '技术分享',
      tagBg: '#fef3c7',
      tagColor: '#d97706',
      title: 'Vue3 性能优化最佳实践',
      subtitle: '深入理解 Vue3 响应式原理，掌握大型应用的性能优化技巧，让你的应用飞速运行，提升用户体验。',
      image: `${storageUrl}/carousel/slide3.jpg`
    }
  ]
}

/**
 * 格式化日期
 */
function formatDate(dateStr) {
  if (!dateStr) return ''
  const date = new Date(dateStr)
  return date.toISOString().split('T')[0]
}

/**
 * 获取推荐文本
 */
function getRecommendText(level) {
  const map = {
    1: '推荐',
    2: '超推荐',
    3: '超级棒'
  }
  return map[level] || '推荐'
}

/**
 * 并行加载所有数据
 * @param {boolean} useCache - 是否优先使用缓存
 * @returns {Promise<boolean>} - 是否成功加载
 */
export async function loadAllData(useCache = true) {
  // 检查是否有可用缓存
  if (useCache) {
    const cachedWorks = getCache(CACHE_KEYS.WORKS)
    const cachedResources = getCache(CACHE_KEYS.RESOURCES)
    const cachedCarousel = getCache(CACHE_KEYS.CAROUSEL)
    const cachedFriends = getCache(CACHE_KEYS.FRIENDS)

    if (cachedWorks && cachedResources && cachedCarousel && cachedFriends) {
      // 有缓存，直接使用
      loadingState.works = cachedWorks
      loadingState.resources = cachedResources
      loadingState.carouselCards = cachedCarousel
      loadingState.friends = cachedFriends
      loadingState.isLoading = false

      // 触发首次加载完成回调（使用缓存）
      if (callbacks.onInitialLoad) {
        callbacks.onInitialLoad({
          works: cachedWorks,
          resources: cachedResources,
          carouselCards: cachedCarousel,
          friends: cachedFriends,
          fromCache: true
        })
      }

      // 静默更新数据
      silentUpdate()
      return true
    }
  }

  // 无缓存或不使用缓存，从网络加载
  loadingState.isLoading = true

  try {
    // 并行加载所有数据
    const [works, resources, carousel, friends] = await Promise.all([
      fetchWorksFromSupabase(),
      fetchResourcesFromSupabase(),
      fetchCarouselFromSupabase(),
      fetchFriendsFromSupabase()
    ])

    // 处理数据
    loadingState.works = works || []
    loadingState.resources = resources || []
    loadingState.carouselCards = carousel || getLocalCarouselCards()
    loadingState.friends = friends || []

    // 缓存数据
    setCache(CACHE_KEYS.WORKS, loadingState.works)
    setCache(CACHE_KEYS.RESOURCES, loadingState.resources)
    setCache(CACHE_KEYS.CAROUSEL, loadingState.carouselCards)
    setCache(CACHE_KEYS.FRIENDS, loadingState.friends)

    loadingState.isLoading = false

    // 触发首次加载完成回调
    if (callbacks.onInitialLoad) {
      callbacks.onInitialLoad({
        works: loadingState.works,
        resources: loadingState.resources,
        carouselCards: loadingState.carouselCards,
        friends: loadingState.friends,
        fromCache: false
      })
    }

    return true
  } catch (err) {
    console.error('加载数据失败:', err)
    loadingState.isLoading = false

    // 触发错误回调
    if (callbacks.onError) {
      callbacks.onError(err)
    }

    return false
  }
}

/**
 * 静默更新数据（后台检查更新）
 */
async function silentUpdate() {
  if (loadingState.isSilentUpdating) return

  loadingState.isSilentUpdating = true

  try {
    // 并行获取最新数据
    const [works, resources, carousel, friends] = await Promise.all([
      fetchWorksFromSupabase(),
      fetchResourcesFromSupabase(),
      fetchCarouselFromSupabase(),
      fetchFriendsFromSupabase()
    ])

    let hasUpdate = false

    // 检查作品数据是否有更新
    if (works && JSON.stringify(works) !== JSON.stringify(loadingState.works)) {
      loadingState.works = works
      setCache(CACHE_KEYS.WORKS, works)
      hasUpdate = true
    }

    // 检查资源数据是否有更新
    if (resources && JSON.stringify(resources) !== JSON.stringify(loadingState.resources)) {
      loadingState.resources = resources
      setCache(CACHE_KEYS.RESOURCES, resources)
      hasUpdate = true
    }

    // 检查轮播数据是否有更新
    if (carousel && JSON.stringify(carousel) !== JSON.stringify(loadingState.carouselCards)) {
      loadingState.carouselCards = carousel
      setCache(CACHE_KEYS.CAROUSEL, carousel)
      hasUpdate = true
    }

    // 检查友链数据是否有更新
    if (friends && JSON.stringify(friends) !== JSON.stringify(loadingState.friends)) {
      loadingState.friends = friends
      setCache(CACHE_KEYS.FRIENDS, friends)
      hasUpdate = true
    }

    // 触发静默更新回调
    if (hasUpdate && callbacks.onSilentUpdate) {
      callbacks.onSilentUpdate({
        works: loadingState.works,
        resources: loadingState.resources,
        carouselCards: loadingState.carouselCards,
        friends: loadingState.friends
      })
    }
  } catch (err) {
    console.error('静默更新失败:', err)
  } finally {
    loadingState.isSilentUpdating = false
  }
}

/**
 * 手动刷新所有数据
 */
export async function refreshAllData() {
  loadingState.isLoading = true
  await loadAllData(false)
}

/**
 * 获取作品数据（返回 computed，Vue 响应式）
 */
export function useWorksData() {
  return {
    works: computed(() => loadingState.works),
    isLoading: computed(() => loadingState.isLoading),
    isSilentUpdating: computed(() => loadingState.isSilentUpdating)
  }
}

/**
 * 获取资源数据（返回 computed，Vue 响应式）
 */
export function useResourcesData() {
  return {
    resources: computed(() => loadingState.resources),
    isLoading: computed(() => loadingState.isLoading),
    isSilentUpdating: computed(() => loadingState.isSilentUpdating)
  }
}

/**
 * 获取友链数据（返回 computed，Vue 响应式）
 */
export function useFriendsData() {
  return {
    friends: computed(() => loadingState.friends),
    isLoading: computed(() => loadingState.isLoading),
    isSilentUpdating: computed(() => loadingState.isSilentUpdating)
  }
}

/**
 * 获取轮播数据（返回 computed，Vue 响应式）
 */
export function useCarouselData() {
  return {
    carouselCards: computed(() => loadingState.carouselCards),
    isLoading: computed(() => loadingState.isLoading)
  }
}

/**
 * 根据ID获取单篇作品详情
 */
export async function fetchWorkById(id) {
  try {
    // 首先检查本地缓存是否有该作品
    const cachedWorks = loadingState.works
    const cachedWork = cachedWorks.find(w => String(w.id) === String(id))
    
    if (cachedWork && cachedWork.content) {
      return cachedWork
    }
    
    // 从 Supabase 获取完整数据
    const { data, error } = await supabase
      .from('works')
      .select('*')
      .eq('id', id)
      .single()
    
    if (error || !data) {
      console.error('获取作品详情失败:', error)
      return null
    }
    
    // 转换数据格式
    return {
      id: data.id,
      title: data.title || '',
      author: data.author || '本人',
      time: formatDate(data.created_at),
      category: data.category ? data.category.split(',').map(c => c.trim()).filter(Boolean) : [],
      image: data.image || `${storageUrl}/images/works/workes.jpg`,
      content: data.content || ''
    }
  } catch (err) {
    console.error('获取作品详情异常:', err)
    return null
  }
}

/**
 * 根据ID获取单个资源详情
 */
export async function fetchResourceById(id) {
  try {
    // 首先检查本地缓存
    const cachedResources = loadingState.resources
    const cachedResource = cachedResources.find(r => String(r.id) === String(id))
    
    if (cachedResource && cachedResource.content) {
      return cachedResource
    }
    
    // 从 Supabase 获取完整数据
    const { data, error } = await supabase
      .from('resources')
      .select('*')
      .eq('id', id)
      .single()
    
    if (error || !data) {
      console.error('获取资源详情失败:', error)
      return null
    }
    
    return {
      id: data.id,
      title: data.title || '',
      description: data.description || '',
      author: data.author || '晴宙',
      time: formatDate(data.created_at),
      category: data.category ? data.category.split(',').map(c => c.trim()).filter(Boolean) : [],
      image: data.image || `${storageUrl}/images/resources/unsplash.png`,
      rating: data.rating || 0,
      official_url: data.official_url || '',
      download_url: data.download_url || '',
      devices: data.devices ? data.devices.split(',').map(d => d.trim()).filter(Boolean) : [],
      warning: data.warning || '',
      content: data.content || ''
    }
  } catch (err) {
    console.error('获取资源详情异常:', err)
    return null
  }
}
