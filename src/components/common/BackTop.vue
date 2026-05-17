<template>
  <button 
    id="backToTop"
    class="back-to-top" 
    :class="{ visible: isVisible }"
    @click="scrollToTop"
  >
    <IconArrowUp />
  </button>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import IconArrowUp from '../icons/IconArrowUp.vue'

const isVisible = ref(false)

const handleScroll = () => {
  isVisible.value = window.scrollY > 300
}

const scrollToTop = () => {
  window.scrollTo({
    top: 0,
    behavior: 'smooth'
  })
}

onMounted(() => {
  window.addEventListener('scroll', handleScroll)
})

onUnmounted(() => {
  window.removeEventListener('scroll', handleScroll)
})
</script>

<style>
.back-to-top {
  position: fixed;
  bottom: 25px;
  right: 25px;
  width: 48px;
  height: 48px;
  border-radius: 50%;
  background-color: rgba(46, 255, 122, 0.3);
  color: #fff;
  border: 2px solid #fff;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 0;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.15);
  transition: all 0.4s ease;
  opacity: 0;
  visibility: hidden;
  z-index: 997;
}

.back-to-top.visible {
  opacity: 1;
  visibility: visible;
}

.back-to-top:hover {
  background-color: rgba(46, 255, 122, 0.5);
  transform: translateY(-2px);
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
}

.back-to-top svg {
  width: 20px;
  height: 20px;
  fill: #fff;
}
</style>