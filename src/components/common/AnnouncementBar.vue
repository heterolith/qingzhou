<template>
  <div class="top-notice-bar">
    <span>本站内容为AI创作 | 2026年5月17日已全新升级</span>
    <button class="notice-detail-btn" @click="$emit('open-modal')">查看详情</button>
  </div>
</template>

<script setup>
defineEmits(['open-modal'])
</script>

<style>
.top-notice-bar {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 40px;
  background-color: rgba(0, 0, 0, 0.9);
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  color: #fff;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0 15px;
  z-index: 9999;
  font-size: 14px;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.notice-detail-btn {
  background: transparent;
  color: #fff;
  border: 2px solid #52C41A;
  border-radius: 20px;
  padding: 4px 12px;
  font-size: 12px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s ease;
  margin-left: 10px;
  flex-shrink: 0;
}

.notice-detail-btn:hover {
  background: rgba(82, 196, 26, 0.15);
  border-color: #52C41A;
  color: #fff;
}

@media (max-width: 360px) {
  .top-notice-bar {
    font-size: 11px;
    padding: 0 8px;
  }

  .notice-detail-btn {
    padding: 2px 8px;
    font-size: 10px;
    margin-left: 6px;
  }
}

@media (min-width: 361px) and (max-width: 480px) {
  .top-notice-bar {
    font-size: 12px;
    padding: 0 10px;
  }
}
</style>
