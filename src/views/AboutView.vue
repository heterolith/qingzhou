<template>
  <div class="about-page">
    <!-- 关于页内容 -->
    <section class="about-section">
      <div class="about-container">
        <!-- 标题 -->
        <div class="about-header">
          <h1>关于晴宙博客</h1>
        </div>

        <!-- 主要内容区 -->
        <div class="about-content">
          <!-- 个人核心卡片 -->
          <div class="about-main-card">
            <!-- 头像 -->
            <div class="avatar-section">
              <img :src="profileData.avatar" :alt="profileData.name + '的头像'" class="custom-avatar" @error="onAvatarError">
              <h2>{{ profileData.name }}</h2>
              <div class="personal-tags">
                <span class="personal-tag" v-for="tag in profileData.tags" :key="tag">{{ tag }}</span>
              </div>
            </div>

            <!-- 价值观 -->
            <div class="quote-section">
              <p>{{ profileData.quote }}</p>
            </div>

            <!-- 联系方式 -->
            <div class="contact-section">
              <h3>交流与联系：</h3>
              <div class="contact-links">
                <a href="javascript:void(0)" class="contact-link-btn">
                  <i class="fas fa-envelope"></i>
                  <span>邮箱</span>
                </a>
                <a href="javascript:void(0)" class="contact-link-btn">
                  <i class="fab fa-github"></i>
                  <span>Github</span>
                </a>
              </div>
            </div>
          </div>

          <!-- 网站信息卡片 -->
          <div class="about-info-card">
            <h3 class="info-card-title">博客核心信息</h3>
            
            <div class="info-item">
              <span class="info-label">内容定位</span>
              <span class="info-value">汽修电路 · 前端 · ESP硬件 · AI · 成长</span>
            </div>

            <div class="info-item">
              <span class="info-label">建站时间</span>
              <span class="info-value">2026年 · AI协助搭建</span>
            </div>

            <div class="info-item">
              <span class="info-label">技术栈</span>
              <div class="tech-stack">
                <span class="tech-tag html">HTML/CSS/JS</span>
                <span class="tech-tag python">Python</span>
                <span class="tech-tag esp">ESP8266/32</span>
              </div>
            </div>
          </div>
        </div>

        <!-- 操作按钮卡片 -->
        <div class="actions-card">
          <h3>更多详情</h3>
          <div class="actions-grid">
            <a href="javascript:void(0)" class="action-btn">
              <i class="fas fa-user"></i>
              <span>经历</span>
            </a>
            <a href="javascript:void(0)" class="action-btn">
              <i class="fas fa-code"></i>
              <span>技术</span>
            </a>
            <a href="javascript:void(0)" class="action-btn">
              <i class="fas fa-sitemap"></i>
              <span>网站</span>
            </a>
            <a href="javascript:void(0)" class="action-btn">
              <i class="fas fa-link"></i>
              <span>联系</span>
            </a>
          </div>
        </div>
      </div>
    </section>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { storageUrl } from '../lib/supabase.js'

// 个人信息数据
const profileData = {
  name: '晴宙',
  avatar: `${storageUrl}/avatar/avatar.jpg`,
  tags: ['汽修电路', '前端开发', 'ESP硬件', 'AI应用'],
  quote: '「慢慢来，持续做，让技术落地」'
}

// 头像加载失败处理
const onAvatarError = (e) => {
  e.target.style.backgroundColor = '#cccccc'
}
</script>

<style>
.about-page {
  min-height: 100vh;
  padding-top: 80px;
}

/* 关于页样式 */
.about-section {
  min-height: calc(100vh - 40px);
  padding: 80px 20px;
  background-color: transparent;
  position: relative;
  z-index: 1;
}

.about-container {
  max-width: 1100px;
  margin: 0 auto;
}

.about-header {
  text-align: center;
  margin-bottom: 60px;
  opacity: 0;
  transform: translateY(20px);
  animation: fadeInUp 1s forwards 0.3s;
}

.about-header h1 {
  font-size: clamp(2.5rem, 6vw, 3.5rem);
  color: var(--primary);
  margin-bottom: 15px;
}

.about-content {
  display: grid;
  grid-template-columns: 7fr 3fr;
  gap: 30px;
  margin-bottom: 60px;
}

.about-main-card, .about-info-card {
  background: #fff;
  border-radius: 20px;
  padding: 40px;
  box-shadow: 0 4px 20px rgba(0,0,0,0.08);
  opacity: 0;
  transform: translateY(30px);
  animation: fadeInUp 1s forwards 0.5s;
  color: #1a1a1a;
}

.about-info-card {
  animation-delay: 0.7s;
}

.avatar-section {
  text-align: center;
  margin-bottom: 40px;
}

.avatar-section img {
  width: 150px;
  height: 150px;
  border-radius: 50%;
  object-fit: cover;
  border: 5px solid var(--accent);
  box-shadow: 0 5px 20px rgba(94, 129, 244, 0.3);
  margin-bottom: 20px;
  display: inline-block;
  vertical-align: middle;
}

/* 自定义头像加载失败时的优雅降级 */
.avatar-section img.custom-avatar {
  background-color: #cccccc;
}

.avatar-section h2 {
  font-size: 2rem;
  color: #1a1a1a;
  margin-bottom: 20px;
}

.personal-tags {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  justify-content: center;
}

.personal-tag {
  padding: 8px 20px;
  background: rgba(94, 129, 244, 0.1);
  color: var(--accent);
  border-radius: 20px;
  font-size: 0.9rem;
  font-weight: 500;
}

.quote-section {
  text-align: center;
  padding: 20px 0;
  border-top: 1px solid var(--light-gray, #e0e0e0);
  border-bottom: 1px solid var(--light-gray, #e0e0e0);
  margin: 30px 0;
}

.quote-section p {
  font-style: italic;
  color: #1a1a1a;
  font-size: 1.1rem;
  margin: 0;
}

.contact-section {
  text-align: center;
}

.contact-section h3 {
  font-size: 1.2rem;
  color: #1a1a1a;
  margin-bottom: 20px;
}

.contact-links {
  display: flex;
  justify-content: space-between;
  gap: 20px;
}

.contact-link-btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  flex: 1;
  padding: 10px 20px;
  background: transparent;
  border: 2px solid var(--light-gray, #e0e0e0);
  border-radius: 20px;
  color: #1a1a1a;
  text-decoration: none;
  font-size: 0.95rem;
  transition: all 0.3s ease;
  text-align: center;
}

.contact-link-btn:hover {
  border-color: var(--accent);
  background: rgba(94, 129, 244, 0.08);
  color: var(--accent);
}

.info-card-title {
  font-size: 1.3rem;
  color: #1a1a1a;
  margin-bottom: 30px;
  text-align: center;
  padding-bottom: 15px;
  border-bottom: 2px solid var(--light-gray, #e0e0e0);
}

.info-item {
  margin-bottom: 25px;
  text-align: center;
}

.info-label {
  font-weight: 600;
  color: #1a1a1a;
  margin-bottom: 8px;
  display: block;
}

.info-value {
  color: #1a1a1a;
  font-size: 0.95rem;
  line-height: 1.5;
}

.tech-stack {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  justify-content: center;
  margin-top: 10px;
}

.tech-tag {
  padding: 6px 14px;
  border-radius: 15px;
  font-size: 0.85rem;
  font-weight: 500;
  color: white;
}

.tech-tag.html { background: #6366f1; }
.tech-tag.python { background: #3b82f6; }
.tech-tag.esp { background: #10b981; }

.actions-card {
  background: #fff;
  border-radius: 20px;
  padding: 40px;
  box-shadow: 0 4px 20px rgba(0,0,0,0.08);
  opacity: 0;
  transform: translateY(30px);
  animation: fadeInUp 1s forwards 0.9s;
  color: #1a1a1a;
}

.actions-card h3 {
  font-size: 1.3rem;
  color: #1a1a1a;
  margin-bottom: 30px;
  text-align: center;
}

.actions-grid {
  display: flex;
  flex-wrap: wrap;
  gap: 12px;
  justify-content: center;
}

.action-btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 14px 24px;
  background: transparent;
  border: 2px solid var(--light-gray, #e0e0e0);
  border-radius: 20px;
  color: #1a1a1a;
  text-decoration: none;
  font-size: 0.95rem;
  font-weight: 500;
  transition: all 0.3s ease;
  text-align: center;
}

.action-btn:hover {
  border-color: var(--accent);
  background: rgba(94, 129, 244, 0.08);
  color: var(--accent);
}

.action-btn i {
  font-size: 1rem;
}

/* 动画 */
@keyframes fadeInUp {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* 媒体查询 - 超小屏幕 (360px及以下) */
@media (max-width: 360px) {
  .about-header h1 {
    font-size: clamp(2rem, 10vw, 2.5rem);
  }

  .about-content {
    grid-template-columns: 1fr;
    gap: 25px;
  }

  .about-main-card, .about-info-card, .actions-card {
    padding: 25px 20px;
  }

  .avatar-section img {
    width: 120px;
    height: 120px;
  }

  .avatar-section h2 {
    font-size: 1.5rem;
  }

  .actions-grid {
    grid-template-columns: 1fr;
  }
}

/* 媒体查询 - 小屏幕 (361px - 480px) */
@media (min-width: 361px) and (max-width: 480px) {
  .about-header h1 {
    font-size: clamp(2.2rem, 9vw, 2.8rem);
  }

  .about-content {
    grid-template-columns: 1fr;
    gap: 25px;
  }

  .actions-grid {
    grid-template-columns: 1fr;
  }
}

/* 媒体查询 - 移动端导航 (992px及以下) */
@media (max-width: 992px) {
  .about-section {
    padding: 60px 20px;
  }

  .about-content {
    grid-template-columns: 1fr;
    gap: 30px;
  }

  .actions-grid {
    grid-template-columns: 1fr;
  }
}

/* 媒体查询 - 超大屏 (1200px及以上) */
@media (min-width: 1200px) {
  .about-container {
    max-width: 1200px;
  }
}
</style>