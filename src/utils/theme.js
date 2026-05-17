// 主题切换工具
// 逻辑：访问时检测时间自动判断，手动切换仅当前会话有效

const THEME_LIGHT = 'light'
const THEME_DARK = 'dark'

// 当前会话的主题状态（内存中，不持久化）
let currentTheme = null

// 根据时间判断应该使用的主题
// 6:00 - 18:00 为白天（亮色），其他时间为晚上（暗色）
function getThemeByTime() {
  const hour = new Date().getHours()
  return hour >= 6 && hour < 18 ? THEME_LIGHT : THEME_DARK
}

// 应用主题到 DOM
function applyTheme(theme) {
  if (theme === THEME_DARK) {
    document.documentElement.classList.add('dark')
  } else {
    document.documentElement.classList.remove('dark')
  }
}

// 初始化主题：检测时间，自动设置
export function initTheme() {
  currentTheme = getThemeByTime()
  applyTheme(currentTheme)
}

// 获取当前主题状态
export function getThemeState() {
  // 如果还没有初始化，根据时间判断
  if (currentTheme === null) {
    currentTheme = getThemeByTime()
  }
  return {
    theme: currentTheme,
    isDark: currentTheme === THEME_DARK
  }
}

// 切换主题：light <-> dark
export function toggleTheme() {
  const { theme } = getThemeState()
  currentTheme = theme === THEME_LIGHT ? THEME_DARK : THEME_LIGHT
  applyTheme(currentTheme)
  return currentTheme
}
