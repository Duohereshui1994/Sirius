#define _USE_MATH_DEFINES
#define MATRIX_SIZE2 2
#define MATRIX_SIZE3 3
#define VECTOR_SIZE 2
#include <Novice.h>
#include <math.h>
#include <assert.h>

//構造体
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




bool  IsSpriteCollision(float sprite1PosX, float sprite1Width, float sprite1PosY, float sprite1Height, float sprite2PosX, float sprite2Width, float sprite2PosY, float sprite2Height) {
	if (sprite1PosX <= sprite2PosX + sprite2Width && sprite2PosX <= sprite1PosX + sprite1Width && sprite1PosY <= sprite2PosY + sprite2Height && sprite2PosY <= sprite1PosY + sprite1Height) {
		return true;
	}
	else {
		return false;
	}
}

bool IsCircleCollision(int distance1, int distance2) {
	if (distance1 > distance2) {
		return true;
	}
	else {
		return false;
	}
}

void SelectionSort(int arr[], int len) {

	for (int i = 0; i < len; i++) {
		int search = i;
		for (int j = i; j < len; j++) {
			if (arr[j] < arr[search]) {
				search = j;
			}
		}
		int temp = arr[search];
		arr[search] = arr[i];
		arr[i] = temp;
	}
}

void InsertionSort(int arr[], int len) {

	for (int i = 1; i < len; i++) {
		int temp = arr[i];
		int j = i - 1;
		while (j >= 0 && arr[j] > temp) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

void BubbleSort(int arr[], int len) {
	int i, j, temp;
	for (i = 0; i < len; i++) {
		for (j = 0; j < len - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

float GetLength(Vector2 start, Vector2 end) {
	return sqrtf((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y));
}

float Length(Vector2 velocity) {
	return sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
}

Vector2 Normalize(Vector2 velocity) {
	float length = 0;
	Vector2 newVelocity = {};

	length = Length(velocity);

	newVelocity.x = velocity.x;
	newVelocity.y = velocity.y;

	if (length != 0.0f) {
		newVelocity.x = velocity.x / length;
		newVelocity.y = velocity.y / length;
	}
	return newVelocity;
}

float DotProduct(Vector2 vector1, Vector2 vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float OuterProduct(Vector2 vector1, Vector2 vector2) {
	return vector1.x * vector2.y - vector1.y * vector2.x;
}


Vector2 ToSCS(Vector2 vector_WCS) {
	Vector2 result;
	result.x = vector_WCS.x;
	result.y = vector_WCS.y * -1 + 500;
	return result;
}

Matrix2x2 Add(Matrix2x2 Matrix1, Matrix2x2 Matrix2) {
	Matrix2x2 result;
	for (int i = 0; i < MATRIX_SIZE2; i++) {
		for (int j = 0; j < MATRIX_SIZE2; j++) {
			result.m[i][j] = Matrix1.m[i][j] + Matrix2.m[i][j];
		}
	}
	return result;
}

Matrix2x2 Subtract(Matrix2x2 Matrix1, Matrix2x2 Matrix2) {
	Matrix2x2 result;
	for (int i = 0; i < MATRIX_SIZE2; i++) {
		for (int j = 0; j < MATRIX_SIZE2; j++) {
			result.m[i][j] = Matrix1.m[i][j] - Matrix2.m[i][j];
		}
	}
	return result;
}

//積
//Matrix2x2 Multiply(Matrix2x2 Matrix1, Matrix2x2 Matrix2) {
//	Matrix2x2 result;
//	result.m[0][0] = Matrix1.m[0][0] * Matrix2.m[0][0] + Matrix1.m[0][1] * Matrix2.m[1][0];
//	result.m[0][1] = Matrix1.m[0][0] * Matrix2.m[0][1] + Matrix1.m[0][1] * Matrix2.m[1][1];
//	result.m[1][0] = Matrix1.m[1][0] * Matrix2.m[0][0] + Matrix1.m[1][1] * Matrix2.m[1][0];
//	result.m[1][1] = Matrix1.m[1][0] * Matrix2.m[0][1] + Matrix1.m[1][1] * Matrix2.m[1][1];
//	return result;
//}


Matrix2x2 Multiply(Matrix2x2 Matrix1, Matrix2x2 Matrix2) {
	Matrix2x2 result;
	for (int i = 0; i < MATRIX_SIZE2; i++) {
		for (int j = 0; j < MATRIX_SIZE2; j++) {
			result.m[i][j] = 0; // 初始化结果矩阵的元素为0
			for (int k = 0; k < MATRIX_SIZE2; k++) {
				result.m[i][j] += Matrix1.m[i][k] * Matrix2.m[k][j];
			}
		}
	}
	return result;
}

Matrix3x3 Multiply(Matrix3x3 Matrix1, Matrix3x3 Matrix2) {
	Matrix3x3 result;
	for (int i = 0; i < MATRIX_SIZE3; i++) {
		for (int j = 0; j < MATRIX_SIZE3; j++) {
			result.m[i][j] = 0; // 初始化结果矩阵的元素为0
			for (int k = 0; k < MATRIX_SIZE3; k++) {
				result.m[i][j] += Matrix1.m[i][k] * Matrix2.m[k][j];
			}
		}
	}
	return result;
}

Vector2 Multiply(Vector2 vector, Matrix2x2 Matrix1) {
	Vector2 result;
	result.x = vector.x * Matrix1.m[0][0] + vector.y * Matrix1.m[1][0];
	result.y = vector.x * Matrix1.m[0][1] + vector.y * Matrix1.m[1][1];
	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 44;
void MatrixSceenPrintf(int x, int y, Matrix2x2 matrix) {
	for (int row = 0; row < 2; ++row) {
		for (int column = 0; column < 2; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth,
				y + row * kRowHeight,
				"%.02f", matrix.m[row][column]
			);
		}
	}
}

void VectorSceenPrintf(int x, int y, Vector2 vector) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
}

Matrix2x2 MakeRotateMatrix2x2(float theta) {
	Matrix2x2 result;
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	return result;
}

Matrix3x3 MakeRotateMatrix3x3(float theta) {
	Matrix3x3 result;
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[0][2] = 0;
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[1][2] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 result;
	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;
	return result;
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Matrix3x3 MakeScaleMatrix(Vector2 scale) {
	Matrix3x3 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate) {
	Matrix3x3 result;
	result.m[0][0] = scale.x * cosf(rotate);
	result.m[0][1] = scale.x * sinf(rotate);
	result.m[0][2] = 0;
	result.m[1][0] = -scale.y * sinf(rotate);
	result.m[1][1] = scale.y * cosf(rotate);
	result.m[1][2] = 0;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;
	return result;
}

Matrix2x2 Inverse(Matrix2x2 matrix) {
	Matrix2x2 result = {};
	float A = matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0];
	if (A != 0) {
		result.m[0][0] = 1.0f / A * matrix.m[1][1];
		result.m[0][1] = 1.0f / A * matrix.m[0][1] * -1;
		result.m[1][0] = 1.0f / A * matrix.m[1][0] * -1;
		result.m[1][1] = 1.0f / A * matrix.m[0][0];
	}
	return result;
}

Matrix3x3 Inverse(Matrix3x3 matrix) {
	Matrix3x3 result = {};
	float A = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];
	if (A != 0) {
		result.m[0][0] = 1.0f / A * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
		result.m[0][1] = 1.0f / A * (matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) * -1;
		result.m[0][2] = 1.0f / A * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
		result.m[1][0] = 1.0f / A * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) * -1;
		result.m[1][1] = 1.0f / A * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
		result.m[1][2] = 1.0f / A * (matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) * -1;
		result.m[2][0] = 1.0f / A * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
		result.m[2][1] = 1.0f / A * (matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) * -1;
		result.m[2][2] = 1.0f / A * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);
	}
	return result;
}

Matrix2x2 Transpose(Matrix2x2 matrix) {
	Matrix2x2 result = {};
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result.m[i][j] = matrix.m[j][i];
		}
	}
	return result;
}
Matrix3x3 Transpose(Matrix3x3 matrix) {
	Matrix3x3 result = {};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = matrix.m[j][i];
		}
	}
	return result;
}