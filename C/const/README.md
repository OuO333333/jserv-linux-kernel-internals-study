# C/C++ 指針常量性總結

| **聲明**               | **指針是否可改變（指向地址）** | **所指向的值是否可改變** | **解釋**                     |
|------------------------|-------------------------------|--------------------------|------------------------------|
| `const int *a`         | 可以                         | 不可以                  | 指向 `const int` 的指針        |
| `int *const a`         | 不可以                       | 可以                    | 常量指針，指向 `int`           |
| `const int *const a`   | 不可以                       | 不可以                  | 常量指針，指向 `const int`      |

---

## 詳細解釋

### 1. `const int *a`
- **解釋**：`a` 是一個指向 `const int` 的指針。
  - 指針本身可以改變，指向其他變量。
  - 不能通過指針修改所指變量的值。
- **示例**：
  ```c
  const int x = 10;
  const int y = 20;
  const int *a = &x; // a 指向 x
  a = &y;            // 合法，指針可以重新指向 y
  *a = 30;           // 非法，不能修改 a 所指的值
