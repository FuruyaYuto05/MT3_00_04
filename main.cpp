#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "MT3_00_04";

// ---------- 構造体定義 ----------
struct Vector3 {
    float x;
    float y;
    float z;
};

struct Matrix4x4 {
    float m[4][4];
};

Matrix4x4 MakeRotateXMatrix(float radian) {
    Matrix4x4 matrix = { 0 };
    matrix.m[0][0] = 1;
    matrix.m[1][1] = cos(radian);
    matrix.m[1][2] = sin(radian);
    matrix.m[2][1] = -sin(radian);
    matrix.m[2][2] = cos(radian);
    matrix.m[3][3] = 1;
    return matrix;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
    Matrix4x4 matrix = { 0 };
    matrix.m[0][0] = cos(radian);
    matrix.m[0][2] = -sin(radian);
    matrix.m[1][1] = 1;
    matrix.m[2][0] = sin(radian);
    matrix.m[2][2] = cos(radian);
    matrix.m[3][3] = 1;
    return matrix;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
    Matrix4x4 matrix = { 0 };
    matrix.m[0][0] = cos(radian);
    matrix.m[0][1] = sin(radian);
    matrix.m[1][0] = -sin(radian);
    matrix.m[1][1] = cos(radian);
    matrix.m[2][2] = 1;
    matrix.m[3][3] = 1;
    return matrix;
}

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result = { 0 }; // 初期化
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int k = 0; k < 4; ++k) {
                result.m[row][col] += a.m[row][k] * b.m[k][col];
            }
        }
    }
    return result;
}

// ---------- 描画用ユーティリティ ----------
static const int kColumnWidth = 60;
static const int kRowHeight = 20;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", vector.x);
    Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
    Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
    Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; column++) {
            Novice::ScreenPrintf(
                x + column * kColumnWidth,
                y + (row + 1) * kRowHeight,
                "%6.02f", matrix.m[row][column]
            );
        }
    }
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Vector3 rotate{ 0.4f, 1.43f, -0.8f };
        Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
        Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
        Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
        Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

        // 回転行列の表示
        MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
