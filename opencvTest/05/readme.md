
## 边缘检测

边缘检测就是检测识别出图像中像素变化剧烈的点的集合。

根据边缘形成的原因，对图像的像素点求微分或二阶微分就可以求出图像中变化剧烈的点。图像边缘就是一阶微分的极大值，也就是二阶微分的零值。
所以，边缘检测可分为如下三种：

> 基于一阶微分的边缘检测，如 Soble算子，Prewite算子，Roberts算子。

> 基于二阶微分的边缘检测，如 拉普拉斯算子，高斯拉普拉斯算子，Canny算子。

> 混合一阶和二阶微分的边缘检测，如 Marr-Hildreth算子。

