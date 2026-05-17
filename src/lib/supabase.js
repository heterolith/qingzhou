import { createClient } from '@supabase/supabase-js'

// Supabase 配置信息
const supabaseUrl = 'https://pilvocmbhhgywprdywkx.supabase.co'
const supabaseAnonKey = 'sb_publishable_0thRWWNhcSUqO2uOTr6_Sg_T7Efa85U'

// 创建 Supabase 客户端
export const supabase = createClient(supabaseUrl, supabaseAnonKey)

// Supabase Storage 公开访问基础 URL
export const storageUrl = `${supabaseUrl}/storage/v1/object/public/images`
