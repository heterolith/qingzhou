<template>
  <div class="friends-container">
    <div class="container">
      <div class="title-box">
        <h2>友情链接</h2>
        <div class="tip-text">友链互加地址请前往<router-link to="/" @click.native="scrollToContact">联系页</router-link></div>
      </div>

      <div v-if="friends.length > 0" class="friends-wrap">
        <a 
          v-for="friend in friends" 
          :key="friend.id"
          :href="friend.url" 
          target="_blank" 
          rel="noopener noreferrer"
          class="friend-item"
        >
          <img class="friend-avatar" :src="friend.avatar" :alt="friend.name" @error="onAvatarError">
          <div class="friend-info">
            <div class="friend-name">{{ friend.name }}</div>
            <div class="friend-desc">{{ friend.desc }}</div>
          </div>
        </a>
      </div>

      <div v-else class="empty-state">
        <p>暂无友链，敬请期待~</p>
      </div>
    </div>
  </div>
</template>

<script setup>
import { useFriendsData } from '../utils/dataLoader.js'

const { friends } = useFriendsData()

// 头像加载失败占位
const onAvatarError = (e) => {
  e.target.src = 'data:image/svg+xml,' + encodeURIComponent('<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 52 52"><rect fill="#e8edf2" width="52" height="52" rx="26"/><text x="26" y="32" text-anchor="middle" fill="#b8d4f0" font-size="20">?</text></svg>')
}

// 跳转联系区
const scrollToContact = () => {
  const el = document.getElementById('contact')
  if (el) {
    const headerHeight = 110
    window.scrollTo({ top: el.offsetTop - headerHeight, behavior: 'smooth' })
  }
}
</script>

<style scoped>
.friends-container {
  min-height: 100vh;
  padding: 40px 15px;
}

.container {
  max-width: 900px;
  margin: 0 auto;
}

.title-box {
  text-align: center;
  margin-bottom: 30px;
}

.title-box h2 {
  font-size: 26px;
  color: var(--text-primary, #222);
  margin-bottom: 10px;
}

.title-box .tip-text {
  font-size: 14px;
  color: var(--text-muted, #777);
}

.title-box .tip-text a {
  color: var(--accent, #0066cc);
  text-decoration: underline;
}

.friends-wrap {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(260px, 1fr));
  gap: 32px;
}

.friend-item {
  width: 260px;
  height: 90px;
  margin: 0 auto;
  background: var(--bg-card, #fff);
  padding: 15px;
  border-radius: 40px;
  border: 1px solid var(--border-color, #eee);
  display: flex;
  align-items: center;
  gap: 15px;
  max-width: 260px;
  text-decoration: none;
  transition: transform 0.2s ease, box-shadow 0.2s ease;
}

.friend-item:hover {
  transform: translateY(-2px);
  box-shadow: var(--shadow-sm, 0 2px 8px rgba(0,0,0,0.08));
}

.friend-avatar {
  width: 52px;
  height: 52px;
  border-radius: 50%;
  flex-shrink: 0;
  background: #e8edf2;
  border: 2px solid #b8d4f0;
  margin-left: 2px;
}

.friend-info {
  display: flex;
  flex-direction: column;
  gap: 5px;
  overflow: hidden;
}

.friend-name {
  font-size: 14px;
  color: var(--text-primary, #111);
  font-weight: 500;
}

.friend-desc {
  width: 165px;
  font-size: 12px;
  color: var(--text-secondary, #666);
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  display: block;
}

.empty-state {
  text-align: center;
  padding: 60px 0;
  color: var(--text-muted, #999);
  font-size: 14px;
}

/* 暗色模式 */
:global(.dark) .friend-item {
  border-color: var(--border-color, #333);
}

:global(.dark) .friend-avatar {
  border-color: rgba(94, 129, 244, 0.4);
}
</style>
