/**
 * 数据缓存工具
 * 提供 localStorage 缓存功能，支持过期时间
 */

// 缓存有效期（毫秒）- 默认30分钟
const DEFAULT_CACHE_DURATION = 30 * 60 * 1000

// 缓存键名
const CACHE_KEYS = {
  WORKS: 'qingzhou_works_data',
  RESOURCES: 'qingzhou_resources_data',
  CAROUSEL: 'qingzhou_carousel_data',
  FRIENDS: 'qingzhou_friends_data',
  CACHE_TIMESTAMP: 'qingzhou_cache_timestamp'
}

/**
 * 存储数据到 localStorage
 * @param {string} key - 缓存键名
 * @param {any} data - 要缓存的数据
 * @param {number} duration - 缓存有效期（毫秒）
 */
export function setCache(key, data, duration = DEFAULT_CACHE_DURATION) {
  try {
    const cacheData = {
      data,
      timestamp: Date.now(),
      duration
    }
    localStorage.setItem(key, JSON.stringify(cacheData))
    return true
  } catch (e) {
    console.warn('缓存存储失败:', e)
    return false
  }
}

/**
 * 从 localStorage 获取数据
 * @param {string} key - 缓存键名
 * @returns {any|null} - 缓存的数据，如果过期或不存在则返回 null
 */
export function getCache(key) {
  try {
    const cached = localStorage.getItem(key)
    if (!cached) return null

    const { data, timestamp, duration } = JSON.parse(cached)
    const now = Date.now()

    // 检查是否过期
    if (now - timestamp > duration) {
      // 缓存已过期，删除它
      localStorage.removeItem(key)
      return null
    }

    return data
  } catch (e) {
    console.warn('缓存读取失败:', e)
    return null
  }
}

/**
 * 删除指定缓存
 * @param {string} key - 缓存键名
 */
export function removeCache(key) {
  try {
    localStorage.removeItem(key)
    return true
  } catch (e) {
    return false
  }
}

/**
 * 检查缓存是否过期
 * @param {string} key - 缓存键名
 * @returns {boolean} - 是否过期或不存在
 */
export function isCacheExpired(key) {
  try {
    const cached = localStorage.getItem(key)
    if (!cached) return true

    const { timestamp, duration } = JSON.parse(cached)
    return Date.now() - timestamp > duration
  } catch (e) {
    return true
  }
}

/**
 * 获取缓存的剩余有效时间
 * @param {string} key - 缓存键名
 * @returns {number} - 剩余时间（毫秒），如果不存在或已过期返回 0
 */
export function getCacheRemainingTime(key) {
  try {
    const cached = localStorage.getItem(key)
    if (!cached) return 0

    const { timestamp, duration } = JSON.parse(cached)
    const remaining = duration - (Date.now() - timestamp)
    return Math.max(0, remaining)
  } catch (e) {
    return 0
  }
}

/**
 * 清除所有应用缓存
 */
export function clearAllCache() {
  Object.values(CACHE_KEYS).forEach(key => {
    localStorage.removeItem(key)
  })
}

// 导出缓存键名常量
export { CACHE_KEYS, DEFAULT_CACHE_DURATION }
