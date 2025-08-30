/**
 * 获取当前时间并格式化为 YYYY-MM-DD HH:mm:ss 的工具方法
 * @returns {string} 格式化后的当前时间字符串
 */
function getCurrentTimeFormatted() {
    const now = new Date();
    const year = now.getFullYear();
    const month = String(now.getMonth() + 1).padStart(2, '0'); // 月份从0开始，需要加1，并补零
    const day = String(now.getDate()).padStart(2, '0'); // 补零
    const hours = String(now.getHours()).padStart(2, '0'); // 补零
    const minutes = String(now.getMinutes()).padStart(2, '0'); // 补零
    const seconds = String(now.getSeconds()).padStart(2, '0'); // 补零
 
    return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

export{
	getCurrentTimeFormatted
}