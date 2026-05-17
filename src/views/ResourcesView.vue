<template>
  <div class="resources-page">
    <!-- 资源展示区域 -->
    <section class="resources-section">
      <div class="container">
        <!-- 搜索和分类 -->
        <div class="top-search-category">
          <div class="search-box">
            <input 
              type="text" 
              class="search-input" 
              placeholder="搜索资源、工具、关键词..."
              v-model="searchKeyword"
            >
          </div>
          <div class="category-list">
            <button 
              v-for="cat in categories" 
              :key="cat.value"
              class="category-item"
              :class="{ selected: currentCategory === cat.value }"
              @click="toggleCategory(cat.value)"
            >
              {{ cat.label }}
            </button>
          </div>
        </div>

        <!-- 资源卡片容器 -->
        <div class="cards-container">
          <router-link 
            v-for="resource in filteredResources" 
            :key="resource.id"
            :to="'/resources/' + resource.id"
            class="resource-card"
            :class="{ hidden: false, selected: false }"
          >
            <div class="resource-info">
              <div class="card-header">
                <div class="card-icon">
                  <img :src="resource.icon" :alt="resource.title" @error="onIconError">
                </div>
                <div class="card-title-wrapper">
                  <div class="card-title" v-html="highlightText(resource.title)"></div>
                  <div class="card-link">{{ resource.link }}</div>
                </div>
              </div>
              <div class="card-recommend" :class="getRecommendClass(resource.recommend)">{{ resource.recommend }}</div>
              <div class="card-desc-wrap">
                <div class="card-desc" v-html="highlightText(resource.description)"></div>
              </div>
              <div class="card-footer">
                <div class="card-tags">
                  <span 
                    v-for="(tag, index) in resource.tags" 
                    :key="index"
                    class="tag"
                    :class="getTagClass(tag)"
                  >{{ tag }}</span>
                </div>
                <div class="card-time">{{ resource.time }}</div>
              </div>
            </div>
          </router-link>
        </div>

        <!-- 无结果提示 -->
        <div class="no-results" v-if="filteredResources.length === 0">
          <div class="no-results-icon">
            <svg viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
              <path d="M446.112323 177.545051c137.567677 0.219798 252.612525 104.59798 266.162424 241.493333 13.562828 136.895354-78.778182 261.818182-213.617777 289.008485-134.852525 27.203232-268.386263-52.156768-308.945455-183.608889s25.018182-272.252121 151.738182-325.779394A267.235556 267.235556 0 0 1 446.112323 177.545051m0-62.060607c-182.794343 0-330.989899 148.195556-330.989899 330.989899s148.195556 330.989899 330.989899 330.989899 330.989899-148.195556 330.989899-330.989899-148.195556-330.989899-330.989899-330.989899z m431.321212 793.341415a30.849293 30.849293 0 0 1-21.94101-9.102223l-157.220202-157.220202c-11.752727-12.179394-11.584646-31.534545 0.37495-43.50707 11.972525-11.972525 31.327677-12.140606 43.494141-0.37495l157.220202 157.220202a31.036768 31.036768 0 0 1 6.723232 33.810101 31.004444 31.004444 0 0 1-28.651313 19.174142z m0 0"/>
            </svg>
          </div>
          <div class="no-results-title">无匹配结果</div>
          <div class="no-results-desc">尝试其他关键词或分类</div>
          <button class="no-results-btn" @click="clearFilter">清空筛选</button>
        </div>
      </div>
    </section>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useResourcesData, registerCallbacks } from '../utils/dataLoader.js'
import { storageUrl } from '../lib/supabase'

// 使用全局缓存的数据 - useResourcesData() 现在返回 computed
const { resources, isLoading } = useResourcesData()

// 搜索关键词
const searchKeyword = ref('')

// 当前分类
const currentCategory = ref('all')

// 本地加载状态 - 直接使用全局状态的 computed
const loading = isLoading

// 本地资源数据 - 直接使用全局缓存的 computed
// (resources 已经是从 useResourcesData() 获取的 computed)

// 分类列表 - 按源码
const categories = [
  { label: '全部资源', value: 'all' },
  { label: '工具', value: 'tool' },
  { label: '图片', value: 'image' },
  { label: '前端', value: 'frontend' },
  { label: 'AI', value: 'ai' },
  { label: '素材', value: 'material' },
  { label: '网站', value: 'website' }
]

// 监听全局数据更新
onMounted(() => {
  // 注册静默更新回调，数据更新时自动同步
  registerCallbacks({
    onSilentUpdate: () => {
      // 数据已通过 useResourcesData() 自动同步
    }
  })
})

// 获取推荐文本
const getRecommendText = (level) => {
  const map = {
    1: '推荐',
    2: '超推荐',
    3: '超级棒'
  }
  return map[level] || '推荐'
}

// 格式化日期
const formatDate = (dateStr) => {
  if (!dateStr) return ''
  const date = new Date(dateStr)
  return date.toISOString().split('T')[0]
}

// 拼音映射表 - 按源码（与作品页相同）
const pinyinMap = {
  '阿': 'A', '爱': 'A', '安': 'A', '昂': 'A', '奥': 'A',
  '八': 'B', '爸': 'B', '白': 'B', '百': 'B', '摆': 'B', '半': 'B', '包': 'B',
  '北': 'B', '被': 'B', '本': 'B', '笔': 'B', '比': 'B', '必': 'B', '边': 'B',
  '编': 'B', '标': 'B', '表': 'B', '别': 'B', '宾': 'B', '兵': 'B', '不': 'B', '步': 'B',
  '部': 'B',
  '才': 'C', '菜': 'C', '参': 'C', '餐': 'C', '草': 'C', '层': 'C', '查': 'C',
  '产': 'C', '长': 'C', '常': 'C', '场': 'C', '唱': 'C', '超': 'C', '车': 'C', '成': 'C',
  '承': 'C', '城': 'C', '吃': 'C', '出': 'C', '除': 'C', '处': 'C', '传': 'C', '船': 'C',
  '窗': 'C', '床': 'C', '春': 'C', '词': 'C', '次': 'C', '从': 'C',
  '大': 'D', '打': 'D', '代': 'D', '带': 'D', '待': 'D', '单': 'D', '但': 'D', '蛋': 'D',
  '当': 'D', '导': 'D', '到': 'D', '道': 'D', '得': 'D', '的': 'D', '地': 'D', '登': 'D',
  '等': 'D', '低': 'D', '底': 'D', '点': 'D', '电': 'D', '店': 'D', '调': 'D', '定': 'D',
  '东': 'D', '冬': 'D', '动': 'D', '都': 'D', '度': 'D', '短': 'D', '对': 'D', '多': 'D',
  '二': 'E', '而': 'E', '儿': 'E', '发': 'F', '法': 'F', '反': 'F', '饭': 'F', '方': 'F',
  '房': 'F', '放': 'F', '非': 'F', '飞': 'F', '分': 'F', '风': 'F', '封': 'F', '夫': 'F',
  '服': 'F', '福': 'F', '父': 'F', '该': 'G', '改': 'G', '概': 'G', '甘': 'G', '感': 'G',
  '干': 'G', '刚': 'G', '钢': 'G', '高': 'G', '搞': 'G', '告': 'G', '哥': 'G', '歌': 'G',
  '个': 'G', '给': 'G', '根': 'G', '更': 'G', '工': 'G', '公': 'G', '功': 'G', '共': 'G',
  '构': 'G', '购': 'G', '古': 'G', '谷': 'G', '股': 'G', '故': 'G', '顾': 'G', '刮': 'G',
  '关': 'G', '观': 'G', '管': 'G', '光': 'G', '广': 'G', '规': 'G', '归': 'G', '国': 'G',
  '过': 'G', '哈': 'H', '孩': 'H', '海': 'H', '含': 'H', '汉': 'H', '好': 'H', '号': 'H',
  '喝': 'H', '和': 'H', '合': 'H', '河': 'H', '很': 'H', '黑': 'H', '红': 'H', '后': 'H',
  '候': 'H', '呼': 'H', '乎': 'H', '忽': 'H', '湖': 'H', '互': 'H', '户': 'H', '花': 'H',
  '华': 'H', '化': 'H', '画': 'H', '话': 'H', '欢': 'H', '还': 'H', '会': 'H', '活': 'H',
  '火': 'H', '或': 'H', '机': 'J', '基': 'J', '级': 'J', '极': 'J', '急': 'J', '集': 'J',
  '计': 'J', '记': 'J', '技': 'J', '际': 'J', '家': 'J', '加': 'J', '假': 'J', '价': 'J',
  '架': 'J', '间': 'J', '监': 'J', '减': 'J', '检': 'J', '简': 'J', '建': 'J', '见': 'J',
  '件': 'J', '健': 'J', '将': 'J', '江': 'J', '讲': 'J', '奖': 'J', '交': 'J', '郊': 'J',
  '教': 'J', '接': 'J', '结': 'J', '解': 'J', '借': 'J', '界': 'J', '金': 'J', '今': 'J',
  '进': 'J', '精': 'J', '经': 'J', '景': 'J', '静': 'J', '九': 'J', '就': 'J', '局': 'J',
  '举': 'J', '具': 'J', '觉': 'J', '绝': 'J', '军': 'J', '卡': 'K', '开': 'K', '看': 'K',
  '科': 'K', '可': 'K', '课': 'K', '客': 'K', '肯': 'K', '空': 'K', '口': 'K', '苦': 'K',
  '快': 'K', '款': 'K', '况': 'K', '拉': 'L', '来': 'L', '蓝': 'L', '劳': 'L', '乐': 'L',
  '老': 'L', '了': 'L', '累': 'L', '类': 'L', '冷': 'L', '离': 'L', '礼': 'L', '李': 'L',
  '理': 'L', '立': 'L', '利': 'L', '力': 'L', '历': 'L', '连': 'L', '联': 'L', '练': 'L',
  '两': 'L', '量': 'L', '林': 'L', '临': 'L', '零': 'L', '领': 'L', '流': 'L',
  '留': 'L', '六': 'L', '龙': 'L', '楼': 'L', '路': 'L', '绿': 'L', '乱': 'L', '论': 'L',
  '罗': 'L', '妈': 'M', '马': 'M', '吗': 'M', '买': 'M', '卖': 'M', '满': 'M', '慢': 'M',
  '忙': 'M', '毛': 'M', '美': 'M', '门': 'M', '们': 'M', '米': 'M', '密': 'M', '面': 'M',
  '民': 'M', '明': 'M', '命': 'M', '模': 'M', '磨': 'M', '末': 'M', '母': 'M', '目': 'M',
  '拿': 'N', '那': 'N', '南': 'N', '男': 'N', '内': 'N', '能': 'N', '你': 'N', '年': 'N',
  '鸟': 'N', '牛': 'N', '农': 'N', '努': 'N', '女': 'N', '欧': 'O', '排': 'P', '盘': 'P',
  '判': 'P', '跑': 'P', '配': 'P', '朋': 'P', '平': 'P', '评': 'P', '苹': 'P', '婆': 'P',
  '破': 'P', '七': 'Q', '其': 'Q', '奇': 'Q', '起': 'Q', '气': 'Q', '器': 'Q', '千': 'Q',
  '前': 'Q', '钱': 'Q', '强': 'Q', '桥': 'Q', '乔': 'Q', '巧': 'Q', '切': 'Q', '且': 'Q',
  '青': 'Q', '清': 'Q', '轻': 'Q', '请': 'Q', '庆': 'Q', '穷': 'Q', '求': 'Q', '球': 'Q',
  '区': 'Q', '曲': 'Q', '取': 'Q', '去': 'Q', '全': 'Q', '然': 'R', '让': 'R', '热': 'R',
  '人': 'R', '认': 'R', '任': 'R', '日': 'R', '容': 'R', '肉': 'R', '入': 'R', '三': 'S',
  '色': 'S', '森': 'S', '杀': 'S', '沙': 'S', '山': 'S', '商': 'S', '上': 'S', '尚': 'S',
  '少': 'S', '社': 'S', '设': 'S', '申': 'S', '身': 'S', '深': 'S', '神': 'S', '生': 'S',
  '省': 'S', '师': 'S', '失': 'S', '诗': 'S', '十': 'S', '时': 'S', '实': 'S', '识': 'S',
  '史': 'S', '使': 'S', '始': 'S', '式': 'S', '示': 'S', '世': 'S', '市': 'S', '事': 'S',
  '视': 'S', '试': 'S', '收': 'S', '手': 'S', '首': 'S', '受': 'S', '书': 'S', '输': 'S',
  '术': 'S', '树': 'S', '双': 'S', '水': 'S', '税': 'S', '说': 'S', '思': 'S', '死': 'S',
  '四': 'S', '送': 'S', '算': 'S', '岁': 'S', '所': 'S', '它': 'T', '他': 'T', '她': 'T',
  '台': 'T', '太': 'T', '态': 'T', '谈': 'T', '特': 'T', '提': 'T', '题': 'T', '体': 'T',
  '天': 'T', '添': 'T', '铁': 'T', '听': 'T', '停': 'T', '通': 'T', '同': 'T', '统': 'T',
  '头': 'T', '图': 'T', '土': 'T', '团': 'T', '推': 'T', '外': 'W', '玩': 'W', '完': 'W',
  '晚': 'W', '万': 'W', '王': 'W', '往': 'W', '网': 'W', '望': 'W', '为': 'W', '位': 'W',
  '未': 'W', '味': 'W', '温': 'W', '文': 'W', '问': 'W', '我': 'W', '无': 'W',
  '五': 'W', '物': 'W', '务': 'W', '西': 'X', '系': 'X', '喜': 'X', '戏': 'X', '细': 'X',
  '下': 'X', '夏': 'X', '先': 'X', '显': 'X', '现': 'X', '线': 'X', '限': 'X', '相': 'X',
  '香': 'X', '想': 'X', '向': 'X', '项': 'X', '象': 'X', '小': 'X', '效': 'X', '些': 'X',
  '协': 'X', '写': 'X', '谢': 'X', '心': 'X', '新': 'X', '信': 'X', '行': 'X', '形': 'X',
  '性': 'X', '姓': 'X', '修': 'X', '需': 'X', '许': 'X', '选': 'X', '学': 'X', '雪': 'X',
  '寻': 'X', '讯': 'X', '压': 'Y', '呀': 'Y', '雅': 'Y', '亚': 'Y', '言': 'Y', '研': 'Y',
  '颜': 'Y', '眼': 'Y', '演': 'Y', '验': 'Y', '阳': 'Y', '养': 'Y', '样': 'Y', '要': 'Y',
  '也': 'Y', '业': 'Y', '页': 'Y', '夜': 'Y', '一': 'Y', '医': 'Y', '衣': 'Y', '依': 'Y',
  '移': 'Y', '已': 'Y', '以': 'Y', '艺': 'Y', '议': 'Y', '异': 'Y', '易': 'Y', '意': 'Y',
  '因': 'Y', '音': 'Y', '引': 'Y', '用': 'Y', '优': 'Y', '游': 'Y', '友': 'Y', '有': 'Y',
  '又': 'Y', '右': 'Y', '鱼': 'Y', '语': 'Y', '育': 'Y', '元': 'Y', '原': 'Y', '远': 'Y',
  '院': 'Y', '月': 'Y', '越': 'Y', '云': 'Y', '运': 'Y', '在': 'Z', '再': 'Z', '早': 'Z',
  '造': 'Z', '则': 'Z', '择': 'Z', '怎': 'Z', '增': 'Z', '展': 'Z', '站': 'Z', '张': 'Z',
  '章': 'Z', '长': 'Z', '找': 'Z', '招': 'Z', '者': 'Z', '这': 'Z', '真': 'Z', '正': 'Z',
  '整': 'Z', '政': 'Z', '之': 'Z', '知': 'Z', '直': 'Z', '值': 'Z', '职': 'Z', '指': 'Z',
  '制': 'Z', '质': 'Z', '治': 'Z', '中': 'Z', '终': 'Z', '钟': 'Z', '种': 'Z', '重': 'Z',
  '周': 'Z', '州': 'Z', '主': 'Z', '住': 'Z', '注': 'Z', '助': 'Z', '祝': 'Z', '抓': 'Z',
  '专': 'Z', '转': 'Z', '赚': 'Z', '准': 'Z', '资': 'Z', '子': 'Z', '字': 'Z', '自': 'Z',
  '总': 'Z', '走': 'Z', '组': 'Z', '租': 'Z', '最': 'Z', '作': 'Z', '做': 'Z', '坐': 'Z',
  '座': 'Z'
}

// 获取拼音
const getPinyin = (text) => {
  if (!text || typeof text !== 'string') return ''
  return text.split('').map(char => pinyinMap[char] || '').join('').toUpperCase()
}

// 匹配搜索
const isMatch = (text, keyword) => {
  if (!text || !keyword) return true
  const normalizedKeyword = keyword.toUpperCase().trim()
  if (!normalizedKeyword) return true
  if (text.toUpperCase().includes(normalizedKeyword)) return true
  const pinyin = getPinyin(text)
  return pinyin.includes(normalizedKeyword)
}

// 高亮文本
const highlightText = (text) => {
  if (!text || !searchKeyword.value) return text
  const keyword = searchKeyword.value.trim()
  if (!keyword) return text
  
  const normalizedKeyword = keyword.toUpperCase()
  
  // 直接匹配
  const regexDirect = new RegExp(`(${escapeRegex(keyword)})`, 'gi')
  if (regexDirect.test(text)) {
    return text.replace(regexDirect, '<span class="highlight">$1</span>')
  }
  
  // 拼音匹配
  const pinyin = getPinyin(text)
  if (pinyin.includes(normalizedKeyword)) {
    let matchCount = 0
    let startCharIndex = 0
    let endCharIndex = 0
    
    for (let i = 0; i < text.length; i++) {
      const char = text[i]
      const charPinyin = pinyinMap[char] || ''
      
      if (charPinyin === normalizedKeyword[matchCount]) {
        if (matchCount === 0) startCharIndex = i
        matchCount++
        if (matchCount === normalizedKeyword.length) {
          endCharIndex = i + 1
          break
        }
      } else if (matchCount > 0) {
        matchCount = 0
      }
    }
    
    if (matchCount === normalizedKeyword.length) {
      const before = text.substring(0, startCharIndex)
      const match = text.substring(startCharIndex, endCharIndex)
      const after = text.substring(endCharIndex)
      return before + '<span class="highlight">' + match + '</span>' + after
    }
  }
  
  return text
}

// 转义正则
const escapeRegex = (str) => {
  if (!str) return ''
  return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')
}

// 图标加载失败处理
const onIconError = (e) => {
  e.target.style.display = 'none'
  e.target.parentNode.innerHTML = `<svg viewBox="0 0 24 24"><path d="M14 2H6c-1.1 0-1.99.9-1.99 2L4 20c0 1.1.89 2 1.99 2H18c1.1 0 2-.9 2-2V8l-6-6zm2 16H8v-2h8v2zm0-4H8v-2h8v2zm-3-5V3.5L18.5 9H13z" fill="#999999"/></svg>`
}

// 获取推荐标签样式
const getRecommendClass = (recommend) => {
  const map = {
    '推荐': 'rec-1',
    '超推荐': 'rec-2',
    '超级棒': 'rec-3'
  }
  return map[recommend] || 'rec-1'
}

// 获取标签样式
const getTagClass = (tag) => {
  const map = {
    '工具': 'tag-tool',
    '图片': 'tag-image',
    '前端': 'tag-front',
    'AI': 'tag-ai',
    '素材': 'tag-material',
    '网站': 'tag-web',
    '组件': 'tag-tool',
    '框架': 'tag-front'
  }
  return map[tag] || 'tag-tool'
}

// 切换分类
const toggleCategory = (cat) => {
  if (currentCategory.value === cat) {
    currentCategory.value = 'all'
  } else {
    currentCategory.value = cat
  }
}

// 清空筛选
const clearFilter = () => {
  searchKeyword.value = ''
  currentCategory.value = 'all'
}

// 筛选后的资源
const filteredResources = computed(() => {
  return resources.value.filter(resource => {
    // 分类筛选
    const matchCategory = currentCategory.value === 'all' || resource.category.includes(currentCategory.value)
    
    // 搜索筛选
    const matchSearch = !searchKeyword.value || 
      isMatch(resource.title, searchKeyword.value) || 
      isMatch(resource.description, searchKeyword.value)
    
    return matchCategory && matchSearch
  })
})
</script>

<style scoped>
.resources-page {
  min-height: 100vh;
  padding-top: 110px;
}

.container {
  width: 90%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 20px;
}

section {
  position: relative;
  z-index: 1;
}

/* Hero 区域 */
.hero {
  min-height: 50vh;
  display: flex;
  align-items: center;
  padding: 120px 4% 80px 4%;
}

.hero-content {
  text-align: center;
  max-width: 1000px;
  width: 100%;
  margin: 0 auto;
  padding: 0 20px;
}

.hero-title h2 {
  font-size: clamp(2.5rem, 8vw, 4.8rem);
  font-weight: 600;
  line-height: 1.1;
  color: var(--primary);
  margin: 0;
  opacity: 0;
  transform: translateY(30px);
  animation: fadeInUp 1s forwards 0.3s;
}

/* 搜索和分类 */
.top-search-category {
  width: 100%;
  max-width: 1200px;
  margin: 0 auto 50px;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 20px;
}

.search-box {
  width: 100%;
  max-width: 600px;
}

.search-input {
  width: 100%;
  height: 50px;
  padding: 0 20px;
  border: 2px solid #e0e0e0;
  border-radius: 30px;
  font-size: 16px;
  outline: none;
  background: #fff;
  transition: all 0.3s ease;
}

.search-input:focus {
  border-color: var(--accent);
  box-shadow: 0 0 20px rgba(94, 129, 244, 0.1);
}

.category-list {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 12px;
}

.category-item {
  padding: 10px 24px;
  border: 2px solid #e0e0e0;
  border-radius: 30px;
  background: transparent;
  font-size: 15px;
  color: #666;
  cursor: pointer;
  transition: all 0.3s ease;
  font-weight: 500;
}

.category-item:hover {
  border-color: var(--accent);
  color: var(--accent);
  transform: translateY(-2px);
}

.category-item.selected {
  border-color: var(--accent);
  color: var(--accent);
  background: rgba(94, 129, 244, 0.08);
}

/* 资源卡片容器 */
.resources-section {
  padding: 0 0 100px;
}

.cards-container {
  width: 100%;
  max-width: 1200px;
  margin: 0 auto 80px;
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 40px;
}

/* 高亮样式 */
.highlight {
  color: rgb(255, 139, 70);
  font-weight: bold;
}

/* 资源卡片 */
.resource-card {
  width: 320px;
  flex-shrink: 0;
  border-radius: 16px;
  background: #fff;
  border: 2px solid transparent;
  display: flex;
  flex-direction: column;
  cursor: default;
  position: relative;
  overflow: hidden;
  opacity: 1;
  transform: scale(1) translateY(0);
  transition: all 0.4s cubic-bezier(0.165, 0.84, 0.44, 1);
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.05);
  text-decoration: none;
  color: inherit;
}

.resource-card.hidden {
  opacity: 0;
  transform: scale(0.95) translateY(20px);
  pointer-events: none;
}

.resource-card:hover {
  transform: scale(1.02) translateY(-3px);
  box-shadow: 0 8px 30px rgba(0, 0, 0, 0.1);
}

.resource-card.selected {
  border-color: #1890ff;
  border-width: 2px;
  box-shadow: 0 8px 30px rgba(24, 144, 255, 0.2);
  transform: scale(1.03) translateY(-4px);
  z-index: 1;
}

/* 资源信息区 */
.resource-info {
  padding: 24px;
  display: flex;
  flex-direction: column;
  gap: 12px;
  flex: 1;
  min-height: 0;
}

.card-header {
  display: flex;
  align-items: center;
  gap: 12px;
}

.card-icon {
  width: 48px;
  height: 48px;
  background: #f1f1f1;
  border-radius: 6px;
  display: grid;
  place-items: center;
  font-size: 20px;
  flex-shrink: 0;
  overflow: hidden;
}

.card-icon img {
  width: 100%;
  height: 100%;
  object-fit: contain;
}

.card-icon svg {
  width: 100%;
  height: 100%;
  fill: #999;
}

.card-title-wrapper {
  flex: 1;
}

.card-title {
  font-size: 1.1rem;
  font-weight: 600;
  color: #222 !important;
  line-height: 1.3;
}

.card-link {
  font-size: 0.85rem;
  color: #666;
  margin-top: 4px;
  word-break: break-all;
}

.card-recommend {
  background: transparent;
  border-radius: 20px;
  padding: 4px 12px;
  font-size: 0.85rem;
  width: fit-content;
  flex-shrink: 0;
  font-weight: 500;
}

.rec-1 { border: 1px solid #00b42a; color: #00b42a; }
.rec-2 { border: 1px solid #1890ff; color: #1890ff; }
.rec-3 { border: 1px solid #fa8c16; color: #fa8c16; }

.card-desc-wrap {
  font-size: 0.95rem;
  color: #666;
  line-height: 1.5;
  flex: 1;
  min-height: 0;
}

.card-desc {
  width: 100%;
  display: -webkit-box;
  -webkit-line-clamp: 3;
  -webkit-box-orient: vertical;
  overflow: hidden;
  line-height: 1.5;
}

.card-footer {
  display: flex;
  justify-content: space-between;
  align-items: center;
  width: 100%;
  margin-top: 8px;
}

.card-tags {
  display: flex;
  gap: 8px;
  flex-wrap: wrap;
}

.tag {
  background: transparent;
  border-radius: 15px;
  padding: 4px 10px;
  font-size: 0.8rem;
  font-weight: 500;
}

.tag-tool  { border: 1px solid #1890ff; color: #1890ff; }
.tag-image { border: 1px solid #00b42a; color: #00b42a; }
.tag-front { border: 1px solid #722ed1; color: #722ed1; }
.tag-ai    { border: 1px solid #fa8c16; color: #fa8c16; }
.tag-web   { border: 1px solid #eb2f96; color: #eb2f96; }
.tag-material { border: 1px solid #13c2c2; color: #13c2c2; }

.card-time {
  font-size: 0.85rem;
  color: #999;
}

.resource-actions {
  display: flex;
  gap: 12px;
  margin-top: 8px;
}

.visit-btn {
  flex: 1;
  padding: 10px 16px;
  border: none;
  border-radius: 20px;
  background: linear-gradient(135deg, var(--accent-color), var(--accent-color-light));
  color: #fff;
  font-size: 0.9rem;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s ease;
  text-decoration: none;
  text-align: center;
}

.visit-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 15px rgba(76, 175, 80, 0.3);
}

/* 无结果提示 */
.no-results {
  text-align: center;
  padding: 80px 20px;
  color: #999;
}

.no-results-icon {
  width: 100px;
  height: 100px;
  margin: 0 auto 24px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.no-results-icon svg {
  width: 100%;
  height: 100%;
  fill: #ccc;
}

.no-results-title {
  font-size: 1.5rem;
  font-weight: 600;
  color: #666;
  margin-bottom: 12px;
}

.no-results-desc {
  font-size: 1rem;
  color: #999;
  margin-bottom: 32px;
}

.no-results-btn {
  padding: 12px 32px;
  border: 2px solid var(--accent);
  border-radius: 30px;
  background: transparent;
  color: var(--accent);
  font-size: 1rem;
  cursor: pointer;
  transition: all 0.3s ease;
  font-weight: 500;
}

.no-results-btn:hover {
  background: var(--accent);
  color: #fff;
  transform: translateY(-2px);
  box-shadow: 0 8px 20px rgba(94, 129, 244, 0.3);
}

/* 动画 */
@keyframes fadeInUp {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* 响应式 */
@media (max-width: 992px) {
  .cards-container {
    gap: 30px;
  }
}

@media (max-width: 768px) {
  .hero {
    min-height: 40vh;
    padding: 100px 4% 60px 4%;
  }

  .cards-container {
    gap: 30px;
  }

  .top-search-category {
    gap: 16px;
  }

  .search-input {
    height: 45px;
    font-size: 15px;
  }

  .category-item {
    padding: 8px 20px;
    font-size: 14px;
  }

  .resource-card {
    border-radius: 20px;
  }

  .resource-info {
    padding: 20px;
  }

  .card-title {
    font-size: 1rem;
  }

  .card-desc-wrap {
    font-size: 0.9rem;
  }
}

@media (max-width: 480px) {
  .top-search-category {
    gap: 12px;
    padding: 0 10px;
  }

  .search-input {
    height: 42px;
    font-size: 14px;
    padding: 0 16px;
  }

  .category-list {
    gap: 8px;
  }

  .category-item {
    padding: 7px 16px;
    font-size: 13px;
  }

  .resource-info {
    padding: 16px;
    gap: 10px;
  }

  .card-icon {
    width: 40px;
    height: 40px;
    font-size: 18px;
  }

  .card-title {
    font-size: 0.95rem;
  }

  .card-desc-wrap {
    font-size: 0.85rem;
  }
}
</style>