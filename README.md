# mutils
PHP扩展，常用工具类

## Rate limiting 本地服务器限流类
```php
$solt = 0;  // 使用那个solt比对
$limit = 100; // 每秒访问量上限
$rl = new M_ratelimit($solt, $limit);
if($rl->acquire()) {
    echo "允许访问";
    return true;
} else {
    echo "超过了{$limit}上限了";
    return false;
}
```
