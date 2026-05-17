<template>
  <div 
    class="modal-overlay" 
    :class="{ active: visible }"
    @click.self="$emit('close')"
  >
    <div class="modal-content">
      <button class="modal-close-btn" @click="$emit('close')">×</button>
      <h2 class="modal-title">网站重要公告</h2>
      <div class="notice-scroll">
        <ul class="notice-ul">
          <li>本站所有内容均为AI辅助创作<span class="important">站主仅提供想法</span>。</li>
          <li>相关内容仅用于填补网页信息，不代表站主任何观点与立场。</li>
          <li>若您对内容存在疑问或发现不适宜信息，可通过<a href="#contact" class="contact-link" @click="$emit('close')">站内预留联系方式</a>反馈，我们将及时处理。</li>
          <li><span class="important">本站已于2026年5月17日完成全站全新升级</span>目前仅有少量细节内容待调整完善</li>
          <li>整合各类实用信息资源，覆盖更广泛的内容领域。</li>
          <li>为用户打造更全面、便捷的信息获取平台。升级完成后，诚挚欢迎各位朋友前来探索访问！</li>
          <li><span class="important">【联系方式说明】目前仅邮箱可联系（ineffabl@outlook.com），其他联系方式暂无法提供</span>。</li>
        </ul>
      </div>
    </div>
  </div>
</template>

<script setup>
defineProps({
  visible: {
    type: Boolean,
    default: false
  }
})

defineEmits(['close'])
</script>

<style scoped>
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.6);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 99999;
  opacity: 0;
  visibility: hidden;
  transition: all 0.3s ease;
}

.modal-overlay.active {
  opacity: 1;
  visibility: visible;
}

.modal-content {
  background-color: white;
  border-radius: 20px;
  padding: 40px 25px 25px;
  max-width: 550px;
  width: 95%;
  max-height: 85vh;
  position: relative;
  display: flex;
  flex-direction: column;
}

.modal-title {
  font-size: 22px;
  font-weight: 700;
  margin-bottom: 20px;
  text-align: center;
  color: var(--accent);
  line-height: 1.2;
  width: 100%;
}

.notice-scroll {
  flex: 1;
  overflow-y: auto;
  padding-right: 5px;
}

.notice-ul {
  list-style: disc;
  padding-left: 20px;
  margin: 0;
}

.notice-ul li {
  font-size: 15px;
  line-height: 1.7;
  color: #555;
  margin-bottom: 10px;
  text-indent: 0;
}

.important {
  background-color: var(--notice-color);
  color: #000;
  font-weight: 600;
  padding: 2px 8px;
  border-radius: 4px;
  display: inline-block;
}

.contact-link {
  color: var(--link-color);
  font-weight: 500;
  text-decoration: none;
  border-bottom: 1px dotted var(--link-color);
  padding: 0 2px;
  transition: all 0.3s ease;
}

.contact-link:hover {
  color: #fff;
  background-color: var(--link-color);
  border-bottom: 1px solid var(--link-color);
  border-radius: 2px;
}

.modal-close-btn {
  position: absolute;
  top: 12px;
  right: 12px;
  background: none;
  border: 2px solid #999;
  border-radius: 50%;
  font-size: 20px;
  font-weight: 300;
  color: #999;
  cursor: pointer;
  transition: all 0.3s ease;
  z-index: 10;
  width: 32px;
  height: 32px;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 0;
  font-family: Arial, sans-serif;
}

.modal-close-btn:hover {
  color: var(--notice-color);
  border-color: var(--notice-color);
}

.notice-scroll::-webkit-scrollbar {
  width: 6px;
}

.notice-scroll::-webkit-scrollbar-thumb {
  background-color: #e0e0e0;
  border-radius: 3px;
}

.notice-scroll::-webkit-scrollbar-thumb:hover {
  background-color: var(--notice-color);
}
</style>
