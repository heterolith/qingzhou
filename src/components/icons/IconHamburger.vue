<template>
  <div class="hamburger" :class="{ active: isActive }">
    <span class="hamburger-line"></span>
    <span class="hamburger-line"></span>
    <span class="hamburger-line"></span>
  </div>
</template>

<script setup>
defineProps({
  isActive: {
    type: Boolean,
    default: false
  }
})
</script>

<style scoped>
.hamburger {
  width: 24px;
  height: 20px;
  position: relative;
  cursor: pointer;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
}

.hamburger-line {
  display: block;
  width: 100%;
  height: 2px;
  background: #333;
  background: var(--primary, #333);
  border-radius: 2px;
  transition: all 0.3s ease;
  transform-origin: center;
}

/* 激活状态 - 变成 X */
.hamburger.active .hamburger-line:nth-child(1) {
  transform: translateY(9px) rotate(45deg);
}

.hamburger.active .hamburger-line:nth-child(2) {
  opacity: 0;
  transform: scaleX(0);
}

.hamburger.active .hamburger-line:nth-child(3) {
  transform: translateY(-9px) rotate(-45deg);
}
</style>
