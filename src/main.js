import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
import { initTheme } from './utils/theme.js'

// 引入全局样式
import './styles/variables.css'
import './styles/reset.css'
import './styles/common.css'

// 初始化主题
initTheme()

const app = createApp(App)

app.use(router)

app.mount('#app')
