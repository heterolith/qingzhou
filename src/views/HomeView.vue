<template>
  <div class="home">
    <!-- Hero 区域 -->
    <section class="hero">
      <div class="container hero-content">
        <div class="hero-text-wrap">
          <div class="hero-title">
            <p>此间种种</p>
            <span style="color: #46FF99;">皆为印记</span>
          </div>
          <div class="hero-buttons">
            <a href="javascript:void(0)" class="btn btn-primary" @click="scrollTo('carousel')">查看作品</a>
            <a href="javascript:void(0)" class="btn btn-secondary" @click="scrollTo('contact')">联系我</a>
          </div>
        </div>
        <div class="hero-avatar">
          <div class="avatar-img" :style="{ backgroundImage: `url(${storageUrl}/avatar/avatar.jpg)` }"></div>
        </div>
      </div>
    </section>

    <!-- 每日一言 -->
    <section class="daily-quote">
      <div class="daily-quote-content">
        <div class="quote-text">
          {{ dailyQuote }}
        </div>
        <div class="quote-divider"></div>
      </div>
    </section>

    <!-- 卡片轮播 -->
    <section class="carousel-section" id="carousel">
      <div class="container carousel-container">
        <div class="carousel">
          <div class="cards-container" ref="cardsContainer">
            <div class="cards-track" ref="cardsTrack" id="cardsTrack">
              <!-- 卡片由JS动态克隆，这里只放原始卡片 -->
              <a 
                v-for="(card, index) in carouselCards" 
                :key="index"
                href="#" 
                class="share-card"
                :data-index="index"
                @click.prevent
              >
                <div class="card-content">
                  <div class="card-left">
                    <span 
                      class="tag" 
                      :style="{ background: card.tagBg, color: card.tagColor }"
                    >
                      {{ card.tag }}
                    </span>
                    <h3 class="card-title">{{ card.title }}</h3>
                    <p class="card-subtitle">{{ card.subtitle }}</p>
                  </div>
                  <div class="card-right">
                    <div class="image-wrapper">
                      <img :src="card.image" :alt="card.title" @error="onImageError">
                    </div>
                  </div>
                </div>
              </a>
            </div>
          </div>

          <!-- 指示器 -->
          <div class="dots-container" ref="dotsContainer" id="dotsContainer">
            <div class="dot active"></div>
            <div class="dot"></div>
            <div class="dot"></div>
          </div>

          <!-- 左右按钮 -->
          <button class="nav-button prev" id="prevBtn" title="上一张" @click.prevent="prev">
            <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
              <path d="M15.41 7.41L14 6l-6 6 6 6 1.41-1.41L10.83 12z"/>
            </svg>
          </button>
          <button class="nav-button next" id="nextBtn" title="下一张" @click.prevent="next">
            <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
              <path d="M10 6L8.59 7.41 13.17 12l-4.58 4.59L10 18l6-6z"/>
            </svg>
          </button>
        </div>
      </div>
    </section>

    <!-- 联系板块 -->
    <section class="contact" id="contact">
      <div class="container">
        <div class="section-title">
          <h2>联系</h2>
        </div>
        <div class="contact-content">
          <div class="contact-info">
            <div class="contact-card qq-card">
              <a href="#" class="contact-link-item" @click.prevent>
                <IconQQ />
                <span>QQ</span>
              </a>
            </div>
            <div class="contact-card email-card">
              <a href="mailto:ineffabl@outlook.com" class="contact-link-item">
                <IconEmail />
                <span>邮箱</span>
              </a>
            </div>
            <div class="contact-card wechat-card">
              <a href="#" class="contact-link-item" @click.prevent>
                <IconWechat />
                <span>微信</span>
              </a>
            </div>
          </div>
        </div>
      </div>
    </section>
  </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted, nextTick, computed, watch } from 'vue'
import IconQQ from '../components/icons/IconQQ.vue'
import IconEmail from '../components/icons/IconEmail.vue'
import IconWechat from '../components/icons/IconWechat.vue'
import { getTodayQuote } from '../lib/quotes.js'
import { storageUrl } from '../lib/supabase.js'
import { useCarouselData, registerCallbacks } from '../utils/dataLoader.js'

// 每日一言
const dailyQuote = getTodayQuote()

// 使用全局缓存的轮播数据 - useCarouselData() 现在返回 computed
const { carouselCards } = useCarouselData()

// 平滑滚动到指定区域
const scrollTo = (sectionId) => {
  const element = document.getElementById(sectionId)
  if (element) {
    const headerHeight = 110
    const targetPosition = element.offsetTop - headerHeight
    window.scrollTo({
      top: targetPosition,
      behavior: 'smooth'
    })
  }
}

const cardCount = computed(() => carouselCards.value.length)

// 轮播状态
const currentIndex = ref(0)
let currentOffset = 0  // 初始化为0，在initCarousel中根据cardCount.value设置
let isAnimating = false
let autoPlayTimer = null
let isCarouselInitialized = false

const cardsTrack = ref(null)
const cardsContainer = ref(null)
const dotsContainer = ref(null)

// 保存原始卡片的引用（在克隆前获取）
let originalCardsRef = null

// 获取卡片宽度
const getCardWidth = () => {
  if (originalCardsRef && originalCardsRef[0]) {
    return originalCardsRef[0].offsetWidth + (window.innerWidth > 768 ? 30 : 20)
  }
  return 930 + (window.innerWidth > 768 ? 30 : 20)
}

// 移动到指定位置
const moveTo = (offset) => {
  if (cardsTrack.value) {
    cardsTrack.value.style.transform = 'translateX(' + (-offset * getCardWidth()) + 'px)'
  }
}

// 更新指示器
const updateDots = (index) => {
  currentIndex.value = index
  if (dotsContainer.value) {
    const dots = dotsContainer.value.querySelectorAll('.dot')
    for (let j = 0; j < dots.length; j++) {
      if (j === index) {
        dots[j].classList.add('active')
      } else {
        dots[j].classList.remove('active')
      }
    }
  }
}

// 自动播放调度
const scheduleNextSwitch = () => {
  if (autoPlayTimer) {
    clearTimeout(autoPlayTimer)
  }
  autoPlayTimer = setTimeout(next, 10000)
}

// 下一张
const next = () => {
  if (isAnimating) return
  isAnimating = true

  if (autoPlayTimer) {
    clearTimeout(autoPlayTimer)
    autoPlayTimer = null
  }

  currentIndex.value = (currentIndex.value + 1) % cardCount.value
  currentOffset++

  moveTo(currentOffset)
  updateDots(currentIndex.value)

  setTimeout(() => {
    isAnimating = false

    if (currentOffset >= cardCount.value * 5) {
      cardsTrack.value.style.transition = 'none'
      currentOffset = cardCount.value * 3
      moveTo(currentOffset)

      setTimeout(() => {
        cardsTrack.value.style.transition = 'transform 0.6s cubic-bezier(0.4, 0.0, 0.2, 1)'
      }, 10)
    }

    scheduleNextSwitch()
  }, 600)
}

// 上一张
const prev = () => {
  if (isAnimating) return
  isAnimating = true

  if (autoPlayTimer) {
    clearTimeout(autoPlayTimer)
    autoPlayTimer = null
  }

  currentIndex.value = (currentIndex.value - 1 + cardCount.value) % cardCount.value
  currentOffset--

  moveTo(currentOffset)
  updateDots(currentIndex.value)

  setTimeout(() => {
    isAnimating = false

    if (currentOffset < cardCount.value * 2) {
      cardsTrack.value.style.transition = 'none'
      currentOffset = cardCount.value * 5 - 1
      moveTo(currentOffset)

      setTimeout(() => {
        cardsTrack.value.style.transition = 'transform 0.6s cubic-bezier(0.4, 0.0, 0.2, 1)'
      }, 10)
    }

    scheduleNextSwitch()
  }, 600)
}

// 跳转到指定卡片
const goTo = (index) => {
  if (isAnimating) return
  isAnimating = true

  if (autoPlayTimer) {
    clearTimeout(autoPlayTimer)
    autoPlayTimer = null
  }

  currentIndex.value = index
  currentOffset = cardCount.value * 3 + index

  moveTo(currentOffset)
  updateDots(currentIndex.value)

  setTimeout(() => {
    isAnimating = false
    scheduleNextSwitch()
  }, 600)
}

// 克隆卡片
const cloneCards = () => {
  if (!cardsTrack.value) return
  
  const track = cardsTrack.value
  
  originalCardsRef = track.querySelectorAll('.share-card')
  
  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < cardCount.value; j++) {
      const clone = originalCardsRef[j].cloneNode(true)
      track.insertBefore(clone, track.firstChild)
    }
  }
  
  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < cardCount.value; j++) {
      const clone = originalCardsRef[j].cloneNode(true)
      track.appendChild(clone)
    }
  }
}

// 触摸滑动
let touchStartX = 0
let touchStartY = 0
const SWIPE_THRESHOLD = 30

const handleTouchStart = (e) => {
  if (isAnimating) return
  touchStartX = e.touches[0].clientX
  touchStartY = e.touches[0].clientY
}

const handleTouchMove = (e) => {
  const touch = e.touches[0]
  const diffX = touchStartX - touch.clientX
  const diffY = touchStartY - touch.clientY

  if (Math.abs(diffX) > Math.abs(diffY)) {
    e.preventDefault()
  }
}

const handleTouchEnd = (e) => {
  const touchEndX = e.changedTouches[0].clientX
  const touchEndY = e.changedTouches[0].clientY

  const diffX = touchStartX - touchEndX
  const diffY = touchStartY - touchEndY

  if (Math.abs(diffX) > Math.abs(diffY) && Math.abs(diffX) > SWIPE_THRESHOLD) {
    if (diffX > 0) {
      next()
    } else {
      prev()
    }
  }
}

// 窗口大小变化
const handleResize = () => {
  moveTo(currentOffset)
}

// 初始化轮播
const initCarousel = () => {
  if (isCarouselInitialized || cardCount.value === 0) return
  isCarouselInitialized = true
  
  nextTick(() => {
    cloneCards()
    
    currentOffset = cardCount.value * 3
    
    if (cardsTrack.value) {
      cardsTrack.value.style.transition = 'none'
      cardsTrack.value.style.transform = 'translateX(' + (-currentOffset * getCardWidth()) + 'px)'
      updateDots(currentIndex.value)
      
      setTimeout(() => {
        if (cardsTrack.value) {
          cardsTrack.value.style.transition = 'transform 0.6s cubic-bezier(0.4, 0.0, 0.2, 1)'
        }
      }, 20)
    }
    
    // 绑定dots点击
    if (dotsContainer.value) {
      const dots = dotsContainer.value.querySelectorAll('.dot')
      for (let j = 0; j < dots.length; j++) {
        (function(dotIndex) {
          dots[dotIndex].addEventListener('click', function(e) {
            e.preventDefault()
            e.stopPropagation()
            goTo(dotIndex)
          })
        })(j)
      }
    }
    
    scheduleNextSwitch()
    
    if (cardsContainer.value) {
      cardsContainer.value.addEventListener('touchstart', handleTouchStart, { passive: true })
      cardsContainer.value.addEventListener('touchmove', handleTouchMove, { passive: false })
      cardsContainer.value.addEventListener('touchend', handleTouchEnd, { passive: true })
    }
    
    window.addEventListener('resize', handleResize)
  })
}

onMounted(() => {
  // 尝试初始化（如果数据已就绪）
  initCarousel()
})

// 监听数据变化，数据到位后初始化轮播
watch(carouselCards, (newVal) => {
  if (newVal && newVal.length > 0) {
    initCarousel()
  }
}, { immediate: false })

onUnmounted(() => {
  if (autoPlayTimer) {
    clearTimeout(autoPlayTimer)
  }
  
  if (cardsContainer.value) {
    cardsContainer.value.removeEventListener('touchstart', handleTouchStart)
    cardsContainer.value.removeEventListener('touchmove', handleTouchMove)
    cardsContainer.value.removeEventListener('touchend', handleTouchEnd)
  }
  
  window.removeEventListener('resize', handleResize)
})
</script>

<style scoped>
.home {
  min-height: 100vh;
}

.container {
  width: 90%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 20px;
}

section {
  padding: 100px 0;
  position: relative;
  z-index: 1;
}

/* Hero 区域 */
.hero {
  min-height: 100vh;
  display: flex;
  align-items: center;
  position: relative;
  overflow: hidden;
  margin-top: 0 !important;
  padding: 60px 4% 30px 4%;
}

.hero-content {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 60px;
  max-width: 1000px;
  width: 100%;
  margin: 0 auto;
  padding: 0 20px;
}

.hero-text-wrap {
  flex: 1;
  max-width: 500px;
  opacity: 0;
  transform: translateY(30px);
  animation: fadeInUp 1s forwards 0.5s;
}

.hero-title {
  margin-bottom: 35px;
}

.hero-title p {
  font-size: clamp(2.5rem, 8vw, 4.8rem);
  font-weight: 600;
  line-height: 1.1;
  color: var(--primary);
  margin: 0;
}

.hero-title span {
  font-size: clamp(1.8rem, 6vw, 3.2rem);
  display: block;
  margin-top: 8px;
}

.hero-avatar {
  flex: 0 0 175px;
  opacity: 0;
  transform: translateY(30px);
  animation: fadeInUp 1s forwards 0.8s;
}

.avatar-img {
  width: 175px;
  height: 175px;
  border-radius: 50%;
  background-color: var(--light-gray);
  background-size: cover;
  background-position: center;
  border: 6px solid #5e81f4;
  box-shadow: 0 0 30px rgba(94, 129, 244, 0.3);
}

.hero-buttons {
  display: flex;
  gap: 18px;
  flex-wrap: wrap;
}

.btn {
  display: inline-block;
  padding: 12px 28px;
  text-decoration: none;
  border-radius: 20px;
  font-weight: 500;
  font-size: 0.95rem;
  transition: all 0.3s ease;
  border: 2px solid #52C41A;
  cursor: pointer;
  background: transparent;
  color: var(--text-primary);
}

.btn:hover {
  border-color: #52C41A;
  color: #52C41A;
  transform: translateY(-2px);
}

.btn-primary,
.btn-secondary {
  background: transparent;
  color: var(--text-primary);
}

.btn-primary:hover,
.btn-secondary:hover {
  border-color: #52C41A;
  color: #52C41A;
  transform: translateY(-2px);
}

/* 每日一言 */
.daily-quote {
  min-height: 60vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 60px 20px;
}

.daily-quote-content {
  text-align: center;
  max-width: 900px;
  padding: 40px 30px;
}

.quote-text {
  font-size: clamp(1.4rem, 3vw, 2.2rem);
  font-weight: 500;
  color: var(--primary);
  line-height: 1.6;
  margin: 0;
  opacity: 0;
  transform: translateY(20px);
  animation: fadeInUp 1s forwards 0.3s;
}

.quote-divider {
  height: 2px;
  background-color: var(--border-color);
  margin: 30px auto 0 auto;
  min-width: 100px;
  max-width: 300px;
  opacity: 0;
  animation: fadeInUp 1s forwards 0.5s;
}

/* 标题样式 */
.section-title {
  position: relative;
  margin-bottom: 55px;
  text-align: center;
}

.section-title h2 {
  font-size: 2.3rem;
  display: inline-block;
  opacity: 0;
  transform: translateY(20px);
  animation: fadeInUp 1s forwards 0.3s;
}

/* 卡片轮播 */
.carousel-section {
  min-height: 70vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 80px 20px;
}

.carousel-container {
  width: 100%;
  max-width: 1200px;
}

.carousel {
  position: relative;
  width: 100%;
  max-width: 900px;
  margin: 0 auto;
}

.cards-container {
  overflow: hidden;
  position: relative;
}

.cards-track {
  display: flex;
  gap: 30px;
  transition: transform 0.6s cubic-bezier(0.4, 0.0, 0.2, 1);
}

.share-card {
  background: var(--bg-card);
  border-radius: 16px;
  width: 100%;
  min-width: 100%;
  flex-shrink: 0;
  overflow: visible;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
  border: 2px solid var(--border-color);
  transition: transform 0.3s ease, box-shadow 0.3s ease, border-color 0.3s ease;
  cursor: pointer;
  text-decoration: none;
  display: block;
}

.share-card:hover {
  box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
  border-color: var(--accent);
}

.card-content {
  display: flex;
  gap: 0;
  min-height: 400px;
}

.card-left {
  flex: 1;
  padding: 50px;
  display: flex;
  flex-direction: column;
  justify-content: center;
}

.card-right {
  width: 50%;
  padding: 40px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.tag {
  display: inline-block;
  font-size: 13px;
  padding: 8px 16px;
  border-radius: 8px;
  margin-bottom: 20px;
  font-weight: 600;
  letter-spacing: 0.5px;
  align-self: flex-start;
}

.card-title {
  font-size: 32px;
  font-weight: 800;
  color: var(--primary);
  margin-bottom: 16px;
  line-height: 1.2;
}

.card-subtitle {
  font-size: 16px;
  color: var(--text-secondary);
  line-height: 1.7;
  margin-bottom: 28px;
}

.image-wrapper {
  width: 100%;
  height: 100%;
  max-height: 320px;
  border-radius: 12px;
  overflow: hidden;
  background-color: var(--light-gray);
}

.image-wrapper img {
  width: 100%;
  height: 100%;
  object-fit: cover;
  display: block;
}

.dots-container {
  display: flex;
  justify-content: center;
  gap: 12px;
  position: absolute;
  bottom: -50px;
  left: 50%;
  transform: translateX(-50%);
  z-index: 10;
}

.dot {
  width: 40px;
  height: 4px;
  background: var(--border-color);
  cursor: pointer;
  transition: all 0.3s ease;
  border-radius: 2px;
}

.dot.active {
  background: var(--accent);
}

.dot:hover {
  background: var(--text-muted);
}

.dot:active {
  transform: scale(0.95);
}

.nav-button {
  position: absolute;
  top: 50%;
  transform: translateY(-50%);
  width: 48px;
  height: 48px;
  background: var(--bg-card);
  border: none;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  z-index: 20;
  transition: all 0.3s ease;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.08);
  backdrop-filter: blur(12px);
}

.nav-button:hover {
  background: var(--bg-secondary);
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.12);
  transform: translateY(-50%) scale(1.05);
}

.nav-button:active {
  transform: translateY(-50%) scale(0.95);
}

.nav-button.prev {
  left: -24px;
}

.nav-button.next {
  right: -24px;
}

.nav-button svg {
  width: 20px;
  height: 20px;
  fill: var(--text-secondary);
  transition: fill 0.3s ease;
}

.nav-button:hover svg {
  fill: var(--accent);
}

/* 联系板块 */
.contact-content {
  width: 100%;
  justify-content: center;
}

.contact-info {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 20px;
  max-width: 800px;
  width: 100%;
  margin: 0 auto;
  justify-content: center;
}

.contact-card {
  border: 1px solid var(--border-color);
  border-radius: 16px;
  transition: all 0.4s cubic-bezier(0.165, 0.84, 0.44, 1);
  opacity: 0;
  transform: translateY(20px);
  animation: fadeInUp 1s forwards 0.3s;
  width: 100%;
  aspect-ratio: 1/1;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;
  background: var(--bg-card);
  min-height: 110px;
  max-width: 180px;
  margin: 0 auto;
}

.contact-card:nth-child(1) {
  animation-delay: 0.3s;
}

.contact-card:nth-child(2) {
  animation-delay: 0.5s;
}

.contact-card:nth-child(3) {
  animation-delay: 0.7s;
}

.qq-card:hover {
  border-color: #FF6B6B;
  box-shadow: 0 5px 15px rgba(255, 107, 107, 0.1);
  transform: translateY(-5px);
}

.email-card:hover {
  border-color: #4ECDC4;
  box-shadow: 0 5px 15px rgba(78, 205, 196, 0.1);
  transform: translateY(-5px);
}

.wechat-card:hover {
  border-color: #52C41A;
  box-shadow: 0 5px 15px rgba(82, 196, 26, 0.1);
  transform: translateY(-5px);
}

.contact-link-item {
  display: flex;
  flex-direction: column;
  gap: 12px;
  text-decoration: none;
  color: var(--primary);
  align-items: center;
  justify-content: center;
  width: 100%;
  height: 100%;
}

.contact-link-item i {
  font-size: 2rem;
  color: var(--accent);
}

.contact-link-item span {
  font-size: 1rem;
  font-weight: 500;
}

/* 动画 */
@keyframes fadeInUp {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* ============ 响应式 ============ */

/* 响应式 */
@media (max-width: 992px) {
  .hero-content {
    flex-direction: column-reverse;
    text-align: center;
    gap: 40px;
  }

  .hero-text-wrap {
    max-width: 100%;
  }

  .hero-buttons {
    justify-content: center;
  }

  .contact-info {
    gap: 15px;
  }

  .contact-card {
    min-height: 100px;
    padding: 18px;
    max-width: 140px;
  }
}

@media (max-width: 768px) {
  .container {
    width: 100%;
    padding: 0 20px;
  }

  section {
    padding: 60px 0;
  }

  .hero {
    padding: 40px 0 20px 0;
    min-height: auto;
  }

  .hero-content {
    padding: 0 20px;
  }

  .carousel-section {
    padding: 40px 0;
    min-height: auto;
  }

  .carousel {
    max-width: 380px;
  }

  .carousel-container {
    padding: 0;
  }

  .cards-track {
    gap: 16px;
  }

  .card-content {
    flex-direction: column;
    min-height: auto;
  }

  .card-left {
    padding: 20px;
    order: 2;
  }

  .card-right {
    width: 100%;
    height: 180px;
    order: 1;
    padding: 16px;
  }

  .image-wrapper {
    border-radius: 12px;
    height: 100%;
    max-height: none;
  }

  .tag {
    font-size: 12px;
    padding: 6px 12px;
    border-radius: 6px;
    margin-bottom: 12px;
  }

  .card-title {
    font-size: 18px;
    margin-bottom: 8px;
    line-height: 1.3;
  }

  .card-subtitle {
    font-size: 14px;
    margin-bottom: 16px;
    display: -webkit-box;
    -webkit-line-clamp: 3;
    -webkit-box-orient: vertical;
    overflow: hidden;
  }

  .nav-button {
    display: none;
  }

  .dots-container {
    bottom: -35px;
    gap: 8px;
  }

  .dot {
    width: 32px;
    height: 3px;
  }

  .daily-quote {
    min-height: auto;
    padding: 40px 16px;
  }

  .daily-quote-content {
    padding: 30px 20px;
  }

  .contact-card {
    max-width: 120px;
    min-height: 100px;
    aspect-ratio: auto;
  }
}

@media (max-width: 480px) {
  .container {
    padding: 0 16px;
  }

  .hero {
    padding: 30px 0 16px 0;
  }

  .hero-content {
    padding: 0 16px;
  }

  .hero-avatar {
    flex: 0 0 140px;
  }

  .avatar-img {
    width: 140px;
    height: 140px;
    border-width: 5px;
  }

  .hero-buttons {
    width: 100%;
    gap: 10px;
  }

  .btn {
    width: 100%;
    padding: 11px 20px;
    font-size: 0.9rem;
  }

  .carousel-section {
    padding: 30px 0;
  }

  .card-left {
    padding: 16px;
  }

  .card-right {
    height: 160px;
    padding: 12px;
  }

  .card-title {
    font-size: 16px;
  }

  .card-subtitle {
    font-size: 13px;
    -webkit-line-clamp: 2;
  }

  .section-title h2 {
    font-size: 1.8rem;
  }

  .daily-quote {
    padding: 30px 12px;
  }

  .daily-quote-content {
    padding: 24px 16px;
  }

  .quote-text {
    font-size: clamp(1.2rem, 4vw, 1.6rem);
  }

  .quote-divider {
    min-width: 80px;
    max-width: 200px;
    margin-top: 24px;
  }

  .contact-card {
    min-height: 90px;
    padding: 12px;
    max-width: 100px;
  }

  .contact-link-item span {
    font-size: 0.85rem;
  }
}

/* 超小屏 */
@media (max-width: 360px) {
  .container {
    padding: 0 12px;
  }

  .hero-content {
    padding: 0 12px;
  }

  .hero-title p {
    font-size: clamp(1.8rem, 10vw, 3rem);
  }

  .hero-title span {
    font-size: clamp(1.3rem, 7vw, 2.2rem);
  }

  .hero-avatar {
    flex: 0 0 110px;
  }

  .avatar-img {
    width: 110px;
    height: 110px;
    border-width: 4px;
  }

  .card-left {
    padding: 14px;
  }

  .card-right {
    height: 140px;
    padding: 10px;
  }

  .card-title {
    font-size: 15px;
  }

  .tag {
    font-size: 11px;
    padding: 4px 10px;
    margin-bottom: 8px;
  }

  .section-title h2 {
    font-size: 1.5rem;
  }

  .contact-card {
    min-height: 80px;
    padding: 10px;
    max-width: 90px;
  }

  .contact-link-item span {
    font-size: 0.8rem;
  }
}

@media (min-width: 1200px) {
  .container {
    max-width: 1250px;
  }

  .hero-content {
    gap: 50px;
  }
}
</style>