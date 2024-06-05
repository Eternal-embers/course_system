# 学生课程管理设计

## 仓库介绍

`data/` 中存放了 `course_info.txt` 作为程序的课程数据来源。课程数据来源于某大学的软件工程的培养方案。

`img/` 中存有程序运行的测试截图。

`src.cpp` 由 `Dev-C++` 编写，因此直接用 `VsCode` 等编辑器打开会出现乱码，可以通过 `.txt` 打开并复制源码则不会出现中文乱码。

## 功能实现

- [x] 查看课程列表
- [x] 根据课程性质(必选/选修)查看课程列表
- [x] 按学分按递减顺序查看课程列表
- [x] 搜索课程
- [x] 查看指定学期的课程列表
- [x] 录入课程
- [x] 重新载入课程

## 运行效果

![启动](./img/start.png)

![查看所有课程信息](./img/view.png)

![根据课程性质查看](./img/view_by_prop.png)

![根据学分递减顺序查看](./img/view_by_credit.png)

![搜索课程](./img/search_by_name.png)

![根据学期查看](./img/search_by_semester.png)

![录入与重新载入课程](./img/input_reload.png)

![异常处理](./img/error.png)

## 未处理的bug

录入完成后，通过重新载入课程信息无法读取到最后一条课程信息。
