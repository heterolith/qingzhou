<template>
  <button class="theme-toggle" @click="$emit('toggle')" :title="title">
    <!-- 太阳图标（亮色模式） -->
    <svg v-if="!isDark" class="theme-icon sun" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
      <circle cx="12" cy="12" r="5" stroke="currentColor" stroke-width="2"/>
      <path d="M12 2v2M12 20v2M4.22 4.22l1.42 1.42M18.36 18.36l1.42 1.42M2 12h2M20 12h2M4.22 19.78l1.42-1.42M18.36 5.64l1.42-1.42" stroke="currentColor" stroke-width="2" stroke-linecap="round"/>
    </svg>
    
    <!-- 月亮图标（暗色模式） -->
    <svg v-else class="theme-icon moon" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
      <path d="M21 12.79A9 9 0 1111.21 3 7 7 0 0021 12.79z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
    </svg>
  </button>
</template>

<script setup>
import { computed } from 'vue'

const props = defineProps({
  preference: {
    type: String,
    default: 'light'
  }
})

defineEmits(['toggle'])

const isDark = computed(() => props.preference === 'dark')

const title = computed(() => {
  return isDark.value ? '当前：暗色模式（点击切换到亮色）' : '当前：亮色模式（点击切换到暗色）'
})
</script>

<style scoped>
.theme-toggle {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 36px;
  height: 36px;
  border: none;
  background: transparent;
  cursor: pointer;
  border-radius: 50%;
  transition: all 0.2s ease;
  color: var(--text-primary, #1a1a1a);
}

.theme-toggle:hover {
  background: var(--bg-secondary, #f8f8f8);
}

.theme-icon {
  width: 20px;
  height: 20px;
  transition: transform 0.3s ease;
}

.theme-toggle:hover .theme-icon {
  transform: rotate(15deg);
}

.theme-toggle:active .theme-icon {
  transform: scale(0.9);
}
</style>
