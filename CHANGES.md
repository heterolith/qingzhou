# 晴宙博客 - 全局加载动画 + 数据缓存机制

## 改动概述

本次更新为「晴宙」Vue 3 博客项目添加了全局加载动画和数据缓存机制，实现了以下功能：

1. **全屏加载动画** - 首次访问时显示美观的加载动画
2. **数据并行预加载** - 应用启动时并行加载所有 Supabase 数据
3. **localStorage 缓存** - 数据缓存到浏览器，支持过期时间
4. **静默更新** - 刷新页面时先使用缓存，后台静默检查更新
5. **平滑过渡** - 加载动画淡出，页面平滑显示

## 新增文件

### 1. `src/components/common/AppLoading.vue`
全局加载动画组件

**功能特性：**
- 全屏加载遮罩，支持亮/暗色主题
- 品牌 Logo 动画（晴宙 / QingZhou）
- 三层同心圆脉冲动画
- 加载提示文字轮换
- 可选进度条显示
- 装饰性几何背景
- 平滑淡入淡出过渡

**使用方式：**
```vue
<AppLoading 
  :show="isLoading" 
  :show-progress="showProgress"
  :progress="loadingProgress"
/>
```

### 2. `src/utils/dataCache.js`
数据缓存工具模块

**功能特性：**
- `setCache(key, data, duration)` - 存储数据到 localStorage
- `getCache(key)` - 获取数据，自动检查过期
- `removeCache(key)` - 删除指定缓存
- `isCacheExpired(key)` - 检查缓存是否过期
- `getCacheRemainingTime(key)` - 获取剩余有效时间
- `clearAllCache()` - 清除所有应用缓存

**缓存键名：**
- `qingzhou_works_data` - 作品数据
- `qingzhou_resources_data` - 资源数据
- `qingzhou_carousel_data` - 轮播数据

**默认缓存时间：** 30 分钟

### 3. `src/utils/dataLoader.js`
数据预加载管理器

**功能特性：**
- `loadAllData(useCache)` - 并行加载所有数据
- `registerCallbacks()` - 注册回调函数
- `silentUpdate()` - 静默更新数据
- `useWorksData()` - 获取作品数据（响应式）
- `useResourcesData()` - 获取资源数据（响应式）
- `useCarouselData()` - 获取轮播数据（响应式）

**加载流程：**
1. 检查 localStorage 是否有有效缓存
2. 有缓存 → 使用缓存 → 后台静默更新
3. 无缓存 → 显示加载动画 → 并行加载所有数据 → 缓存数据 → 关闭动画

## 修改文件

### 1. `src/App.vue`
- 引入并使用 `AppLoading` 组件
- 在应用初始化时调用 `loadAllData()` 并行加载数据
- 管理加载状态（`isLoading`、`showProgress`、`loadingProgress`）
- 注册数据加载回调函数

### 2. `src/views/WorksView.vue`
- 移除独立的 `fetchWorks()` 函数
- 使用 `useWorksData()` 获取全局缓存的作品数据
- 加载状态与全局同步
- 数据更新自动响应

### 3. `src/views/ResourcesView.vue`
- 移除独立的 `fetchResources()` 函数
- 使用 `useResourcesData()` 获取全局缓存的资源数据
- 加载状态与全局同步
- 数据更新自动响应

### 4. `src/views/HomeView.vue`
- 移除本地硬编码的轮播数据
- 使用 `useCarouselData()` 获取全局缓存的轮播数据
- 数据更新自动响应

## 技术实现

### 缓存策略
```
┌─────────────────────────────────────────────────────────┐
│                    应用启动                               │
└─────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────┐
│              检查 localStorage 缓存                      │
└─────────────────────────────────────────────────────────┘
                            │
          ┌─────────────────┴─────────────────┐
          │                                   │
          ▼                                   ▼
    ┌──────────┐                        ┌──────────┐
    │ 有有效缓存 │                        │ 无缓存    │
    └──────────┘                        └──────────┘
          │                                   │
          ▼                                   ▼
    ┌──────────┐                        ┌──────────┐
    │使用缓存   │                        │显示加载动画│
    │直接显示   │                        │并行加载数据│
    └──────────┘                        └──────────┘
          │                                   │
          ▼                                   ▼
    ┌──────────┐                        ┌──────────┐
    │后台静默更新│                        │缓存数据   │
    │检查新数据 │                        │关闭动画   │
    └──────────┘                        └──────────┘
```

### 数据流向
```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│  Supabase    │───▶│  dataLoader   │───▶│ localStorage │
│  (数据库)     │    │  (预加载器)   │    │   (缓存)      │
└──────────────┘    └──────────────┘    └──────────────┘
                           │
                           ▼
                   ┌──────────────┐
                   │  Vue 组件     │
                   │ (Works/Res/   │
                   │  Home)        │
                   └──────────────┘
```

## 使用示例

### 在 Vue 组件中使用缓存数据
```javascript
import { useWorksData } from '../utils/dataLoader.js'

export default {
  setup() {
    const { works, isLoading } = useWorksData()
    return { works, isLoading }
  }
}
```

### 手动刷新数据
```javascript
import { refreshAllData } from '../utils/dataLoader.js'

// 刷新所有数据
await refreshAllData()
```

### 清除缓存
```javascript
import { clearAllCache } from '../utils/dataCache.js'

// 清除所有缓存
clearAllCache()
```

## 注意事项

1. **首次访问** - 首次访问时仍需要等待数据加载完成
2. **缓存过期** - 默认 30 分钟后缓存自动失效
3. **静默更新** - 后台静默更新不会显示额外提示
4. **主题适配** - 加载动画自动适配亮/暗色主题
5. **动画时长** - 加载动画淡出过渡为 0.6 秒

## 兼容性

- Vue 3 Composition API
- localStorage（现代浏览器）
- CSS Variables（支持暗色主题）

## 更新日志

- **2024** - 初始版本，实现全局加载动画和数据缓存机制
