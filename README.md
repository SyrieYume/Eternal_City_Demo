
## 如何运行程序
1. 从 [Releases](https://github.com/SyrieYume/Eternal_City_Demo/releases/latest) 下载 `Eternal_City_Demo.exe`

2. 运行 `Eternal_City_Demo.exe`

3. 如果遇到 **Bug**，欢迎在 [Issues](https://github.com/SyrieYume/Eternal_City_Demo/issues) 中提出

## 如何手动编译本项目
1. 安装C语言编译器 **MinGW** (gcc version 14.2.0)

2. 新建一个空文件目录 (文件路径中最好不要带 中文 / 空格 )

3. 从 [Releases](https://github.com/SyrieYume/Eternal_City_Demo/releases/latest) 下载项目的源代码部分，解压到上述文件目录下

4. 从 [Releases](https://github.com/SyrieYume/Eternal_City_Demo/releases/latest) 下载项目的资源文件部分，并解压到上述文件目录下的 `res` 文件夹中

5. 在 **上述文件目录下** 执行以下命令：

```powershell
windres ./res/res.rc res.o
gcc src/*.c res.o -o Eternal_City_Demo.exe -mwindows -lcomctl32 -lwinmm
rm res.o
```

6. 执行完毕后，就可以在 文件目录 中找到 `Eternal_City_Demo.exe`


## 注意事项
#### 1. 只支持 Windows 平台

#### 2. 编译时，代码源文件以及编译器 **必须** 使用 `UTF-8` 编码

#### 3. 只支持 **.bmp** 格式的图片
   可以使用 图像处理软件(例如:PS) 将图片转换为 **.bmp** 格式

#### 4. 只支持 **.wmv** 格式的视频  
   使用 **ffmpeg** 将 **mp4** 格式视频 转换为 **.wmv** 格式视频 的方法：
   ```bash
    ffmpeg -i 文件名.mp4 -crf 22 -r 30 -q:v 5 -c:v wmv2 -b:v 1500k -c:a wmav2 -b:a 192k -s 1920x1080 文件名.wmv
   ```
   其中：`-r` 参数表示每秒帧数， `-q:v` 表示视频质量（值越大，质量越差，但是视频体积越小），`-s` 表示视频分辨率

