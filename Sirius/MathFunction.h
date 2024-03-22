#pragma once
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

typedef struct Matrix2x2 {
	float m[2][2];
}Matrix2x2;

typedef struct Matrix3x3 {
	float m[3][3];
}Matrix3x3;

/// <summary>
/// 矩形的冲突判定
/// </summary>
/// <param name="sprite1PosX  ">矩形1的横坐标起点</param>
/// <param name="sprite1Width ">矩形1的横向长度</param>
/// <param name="sprite1PosY  ">矩形1的纵坐标起点</param>
/// <param name="sprite1Height">矩形1的纵向长度</param>
/// <param name="sprite2PosX  ">矩形2的横坐标起点</param>
/// <param name="sprite2Width ">矩形2的横向长度</param>
/// <param name="sprite2PosY  ">矩形2的纵坐标起点</param>
/// <param name="sprite2Height">矩形2的纵向长度</param>
/// <returns>0：没冲突  1：冲突</returns>
bool IsSpriteCollision(float sprite1PosX, float sprite1Width, float sprite1PosY, float sprite1Height, float sprite2PosX, float sprite2Width, float sprite2PosY, float sprite2Height);
/// <summary>
/// 两圆冲突判定
///</summary>
///<param name="distance1">円と円の中心の距離</param>
///<param name="distance2">円と円の半径の和</param>
/// <returns>
/// 0：あたっていない 
/// 1：あたる
/// </returns>
bool IsCircleCollision(int distance1, int distance2);
/// <summary>
/// 选择排序
/// </summary>
/// <param name="arr">目标数组</param>
/// <param name="len">目标数组的长度，计算方式：int len = (int)sizeof(arr) / sizeof(*arr);或sizeof(arr) / sizeof(arr[0]);</param>
void SelectionSort(int arr[], int len);
/// <summary>
/// 插入排序
///</summary>
/// <param name="arr">目标数组</param>
/// <param name="len">目标数组的长度，计算方式：int len = (int)sizeof(arr) / sizeof(*arr);或sizeof(arr) / sizeof(arr[0]);</param>
void InsertionSort(int arr[], int len);
/// <summary>冒泡排序</summary>
/// <param name="arr">数组名字</param>
/// <param name="len">数组长度：计算公式 int len = sizeof(arr) / sizeof(arr[0]);</param>
void BubbleSort(int arr[], int len);
///<summary>
/// 求向量间的长度 用的是终点-起点 方向指向终点
///</summary>
///<param name="start">向量的起点（Vector2构造体形式）</param>
///<param name="end">向量的终点（Vector2构造体形式）</param>
///<returns></returns>
float GetLength(Vector2 start, Vector2 end);
/// <summary>
/// 速度正规化时的长度
/// </summary>
/// <param name="velocity">目标速度</param>
/// <returns>长度</returns>
float Length(Vector2 velocity);
/// <summary>
/// 速度正规化，8方向移动时用
/// </summary>
/// <param name="velocity">目标速度</param>
/// <returns>正规化后速度系数 随后乘以原速度</returns>
Vector2 Normalize(Vector2 velocity);
/// <summary>
/// 内积  其实是点积
/// </summary>
/// <param name="vector1">向量1</param>
/// <param name="vector2">向量2</param>
/// <returns>两个向量的内积</returns>
float DotProduct(Vector2 vector1, Vector2 vector2);
/// <summary>
/// 外积
/// </summary>
/// <param name="vector1">向量1</param>
/// <param name="vector2">向量2</param>
/// <returns>向量的外积</returns>
float OuterProduct(Vector2 vector1, Vector2 vector2);
/// <summary>
/// 把WCS坐标转换成SCS坐标
/// </summary>
/// <param name="vector_WCS">要转换的WCS坐标</param>
/// <returns>SCS坐标</returns>
Vector2 ToSCS(Vector2 vector_WCS);
/// <summary>
/// 2x2矩阵加法
/// </summary>
/// <param name="Matrix1">矩阵1</param>
/// <param name="Matrix2">矩阵2</param>
/// <returns>加完的2x2矩阵</returns>
Matrix2x2 Add(Matrix2x2 Matrix1, Matrix2x2 Matrix2);
/// <summary>
/// 2x2矩阵减法
/// </summary>
/// <param name="Matrix1">矩阵1</param>
/// <param name="Matrix2">矩阵2</param>
/// <returns>减完的2x2矩阵</returns>
Matrix2x2 Subtract(Matrix2x2 Matrix1, Matrix2x2 Matrix2);
/// <summary>
/// 2x2矩阵的乘法
/// </summary>
/// <param name="Matrix1">矩阵1</param>
/// <param name="Matrix2">矩阵2</param>
/// <returns>乘完的2x2矩阵</returns>
Matrix2x2 Multiply(Matrix2x2 Matrix1, Matrix2x2 Matrix2);
/// <summary>
/// 3x3矩阵的乘法
/// </summary>
/// <param name="Matrix1">矩阵1</param>
/// <param name="Matrix2">矩阵2</param>
/// <returns>乘完的3x3矩阵</returns>
Matrix3x3 Multiply(Matrix3x3 Matrix1, Matrix3x3 Matrix2);
/// <summary>
/// 2维向量和2x2矩阵的乘法
/// </summary>
/// <param name="vector">2维向量</param>
/// <param name="Matrix1">2x2矩阵</param>
/// <returns>结果是1x2的向量</returns>
Vector2 Multiply(Vector2 vector, Matrix2x2 Matrix1);
/// <summary>
/// 输出2x2矩阵
/// </summary>
/// <param name="x">横坐标</param>
/// <param name="y">纵坐标</param>
/// <param name="matrix">要打印的矩阵</param>
void MatrixSceenPrintf(int x, int y, Matrix2x2 matrix);
/// <summary>
/// 输出1x2向量
/// </summary>
/// <param name="x">横坐标</param>
/// <param name="y">纵坐标</param>
/// <param name="vector">要打印的向量</param>
void VectorSceenPrintf(int x, int y, Vector2 vector);
/// <summary>
/// 制作2x2旋转矩阵 对应点的向量和它相乘，得到旋转后的向量 这个坐标一定是以原点为基准
/// </summary>
/// <param name="theta">旋转角度</param>
/// <returns>返回这个旋转矩阵</returns>
Matrix2x2 MakeRotateMatrix2x2(float theta);
/// <summary>
/// 制作3x3旋转矩阵 对应点的向量和它相乘，得到旋转后的向量 这个坐标一定是以原点为基准
/// </summary>
/// <param name="theta">旋转角度</param>
/// <returns>返回这个旋转矩阵</returns>
Matrix3x3 MakeRotateMatrix3x3(float theta);
/// <summary>
/// 制作平行移动矩阵
/// </summary>
/// <param name="translate">要移动的点的向量 移动时控制这个点的移动就行</param>
/// <returns>生成的平行移动矩阵</returns>
Matrix3x3 MakeTranslateMatrix(Vector2 translate);
/// <summary>
/// 点のベクトルと平行移動行列の積 平行移動を実現する関数
/// </summary>
/// <param name="vector">点のベクトル</param>
/// <param name="matrix">変換した平行移動行列の積</param>
/// <returns>移动后点的向量</returns>
Vector2 Transform(Vector2 vector, Matrix3x3 matrix);
/// <summary>
/// 制作扩缩矩阵
/// </summary>
/// <param name="scale">扩缩的倍率向量，横向纵向的</param>
/// <returns>制作好的扩缩矩阵</returns>
Matrix3x3 MakeScaleMatrix(Vector2 scale);
/// <summary>
/// 扩缩旋转移动 SRT 3 in 1 矩阵
/// </summary>
/// <param name="scale">拡縮倍率 scale.x=k  scale.y=l</param>
/// <param name="rotate">回転のtheta</param>
/// <param name="translate">平行移動</param>
/// <returns>返回这个矩阵</returns>
Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate);
/// <summary>
/// 求2x2矩阵的逆
/// </summary>
/// <param name="matrix">目标矩阵</param>
/// <returns>逆</returns>
Matrix2x2 Inverse(Matrix2x2 matrix);
/// <summary>
///  求3x3矩阵的逆
/// </summary>
/// <param name="matrix">目标矩阵</param>
/// <returns>逆</returns>
Matrix3x3 Inverse(Matrix3x3 matrix);
/// <summary>
/// 求2x2矩阵的转置矩阵
/// </summary>
/// <param name="matrix">目标矩阵</param>
/// <returns>转置矩阵</returns>
Matrix2x2 Transpose(Matrix2x2 matrix);
/// <summary>
/// 求3x3矩阵的转置矩阵
/// </summary>
/// <param name="matrix">目标矩阵</param>
/// <returns>转置矩阵</returns>
Matrix3x3 Transpose(Matrix3x3 matrix);