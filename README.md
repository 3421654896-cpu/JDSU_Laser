# FBG OptiSense Studio / JDSU Laser

面向 STM32F205RE + PI11210 光栅解调仪的 Windows 客户端、嵌入式固件和开发验证工具。
客户端以同一套原始 ADC、标定波长和拟合链路处理 USB / LAN 采集；每台机器的参数与校准记录独立保存。

## 当前版本与仓库状态

截至 **2026-10-10**：客户端源码 **v1.0.7**，应用固件源码 **v1.0.110**。

本次源码检查点基于 `337d332`（`8_6`）：提交前重新拉取确认，本地 `main`、
[`origin/main`](https://github.com/3421654896-cpu/JDSU_Laser) 和
[`upstream/main`](https://github.com/Purer-lyk/JDSU_Laser) 均为该提交，无新增远程提交需要合并。
此检查点收录之后积累的客户端、固件、校准/分析工具、回归测试和部署源码；不是仅导出旧 Git 版本。

**GitHub 源码提交不等于客户版本发布。** 本次没有生成新安装包、更新现有交付 ZIP 或发布 GitHub Release。
现有 v1.0.7 客户交付包中的固件仍为 v1.0.109；v1.0.110 是本次修复并在三号机验证的版本。
历史兼容固件资源及 `usb_firmware_catalog.json` 中的旧条目，不代表最新源码已向客户自动发布。

## 当前客户功能

- 光谱与寻峰：自动校准、单值、手指应力测试；重采密集光谱后默认自动选取 9 峰 × 5 点，选点失败保留原始数据并允许手动选点。
- 手指应力测试：默认连续循环；密集采集启动默认 CH1、1525–1555 nm、0.04 nm、逐点等待 0.001 s；峰内/跨峰等待默认均为 850 µs。
- 实时拟合默认使用 Makima 样条；参考线与实时拟合开关放在“曲线与点位”同一行，不折叠。
- 45 点参考线：每点写入对应 DAC，等待 **0.010 s** 后读取一次新鲜原始 ADC。不按光学稳定性或峰形筛除/补采；保留设备身份、协议、电流边界和关光检查。45 × 10 ms = 0.45 s 只是等待预算，不是保证的整次耗时。
- 左下角显示板温、风扇转速；所有机器及后续固件共用 **40°C** 温控，正常校准不额外强制满转，启动/传感器/PWM 故障全速保护保留。
- USB 和 LAN 数据采集保留；连接目标与校准表按机号隔离，不能用历史 COM 号或 IP 推定当前设备身份。

应力寻峰、温度寻峰、无限准确模式、等间隔模式、MQTT 广域网、机械手指 3D/设备配置侧栏入口、
高斯＋基线按钮、内置客户端/固件更新页及多余状态文字在客户界面隐藏。
机械臂连接提示、恢复/急停按钮及旧“未通过点（红×）”按钮不在当前客户主界面显示。
隐藏界面不意味着共享采集底层可以删除，也不意味着客户主界面提供机械臂急停；使用机械臂研发工具必须另行确认独立安全停止手段。

## 固件 v1.0.110 修复与实测范围

v1.0.109 的临时 45 点协议在校验所选通道前，错误地要求 CH0 一律处于 2 kΩ，
导致合法的 **CH0 + 5/20/40 kΩ** 被 `0x0200` 拒绝。
v1.0.110 改为校验实际所选通道及请求挡位；不通过偷偷将 CH0 改回 2 kΩ 绕过错误。
非选中前端的默认挡位、错误挡位拒绝、CRC、电流限制、光闸和停止握手继续保留。

2026-10-10 三号机通过 CMSIS 烧录并回读验证 v1.0.110，保留引导/配置区和应用未写尾部。
**零电流协议实测**覆盖：

- CH0 / CH1 各 2、40、5、20 kΩ，以及 CH2 / CH3 固定挡位，共 10 种组合，每种接收 32 帧；850/850 µs 默认参数下均通过，无 I²C/SPI 错误。
- 单值、零电流手动校准、旧模式安全拒绝/精确 DISARM、2001 点密集采集、45 点临时流及采集中的温度/风扇遥测。
- 2001 点逐点等待 2 ms 的该次零电流实测总耗时 **5.328 s**，纯等待预算为 4.002 s；存在 DAC、协议和传输开销。
- 测试结束收到关光 ACK，重启标识及固件版本在测试前后保持一致。

以上不是实际光谱精度、带光稳定性或整机校准验收。三号机临时 2001 点表仍有未通过/待测点，
不能称为全光学验收完成；二号机尚不能由三号机的 v1.0.110 测试推定已升级/验收。
详细记录见 [本次验证说明](docs/verification_20261010_fw110.md)。

## 目录

| 路径 | 内容 |
| --- | --- |
| `Python/` | Qt 客户端、共享采集/拟合、机号档案、自动校准/选点、USB 更新、离线助手、分析工具与测试 |
| `JDSU/Core/`、`JDSU/USB_DEVICE/` | 正式应用源码与 USB CDC |
| `JDSU/Bootloader/` | OTA/USB 共用引导与构建脚本 |
| `JDSU/MDK-ARM/`、`JDSU/JDSU.ioc` | Keil 工程、加载配置与 CubeMX 配置 |
| `deployment/` | PyInstaller、Inno Setup、开发源码交接及离线更新流程 |
| `backups/hidden_features_20261009_d8e7f706/` | 精简前可编辑旧功能源码（不是正式构建入口） |
| `backups/reference45_before_10ms_20261010/` | 改为 10 ms 原始 ADC 前的参考线逻辑源码 |

保留必要的标定数据/测试输入和历史版本标签，但这些文件并非全部验收通过。
现场 `outputs/`、运行记录、私有全 Flash 备份、模型大文件、虚拟环境与安装器不上传；
部分研发/分析工具需要另行提供对应的授权数据，不能直接运行所有 `live` 脚本当成单元测试。
`outputs/ota/` 中明确纳入版本控制的少量历史构建兼容资源是例外，不包含现场 Flash 转储。

## 开发环境与安全启动

Windows 10/11 x64、CPython 3.12 x64；依赖以
[`requirements-lock.txt`](deployment/developer_bundle/requirements-lock.txt) 为准。
Windows 建议克隆到较短路径（例如 `C:\src\JDSU_Laser`）；旧源码备份中存在长路径。
如 Git 检出报告 `Filename too long`，在该仓库启用 `git config core.longpaths true` 后重新检出缺失文件，
或首次克隆使用 `git -c core.longpaths=true clone`。不要为此删除备份源码。
在仓库根目录执行：

```powershell
py -3.12 -m venv Python\.venv
.\Python\.venv\Scripts\python.exe -m pip install -r deployment\developer_bundle\requirements-lock.txt
$env:QT_OPENGL = "software"
.\Python\.venv\Scripts\python.exe Python\fbg_studio_launcher.py --demo
```

演示模式不连接/控制硬件。开发入口是 `Python/fbg_studio_launcher.py`，不是旧版独立脚本。
确认设备身份、光路、电流上限及停止手段后，才去掉 `--demo` 进行实际使用。
串口需相应 VCP/CH343 驱动，AQ6150B 需 VISA Runtime；Mach3、SolidWorks 仅对应研发工具需要。
第三方编译器、设备驱动及 SolidWorks 互操作 DLL 由开发者自行合法安装，不随本次源码提交。

## 无硬件回归与固件构建

先从 `Python/` 目录运行，不接管设备：

```powershell
Push-Location Python
.\.venv\Scripts\python.exe -m unittest test_thermal_fan_policy test_firmware_feature_profile test_firmware_scan_scheduler test_temporary_feedback_policy test_temporary_test_mode test_validate_production_trim_live test_guarded_application_upgrade
.\.venv\Scripts\python.exe -m unittest test_temporary_reference test_dense_scan_transport test_temporary_test_widget test_fbg_unified_app test_ui_responsive_layout test_requested_client_changes test_board_thermal_protocol test_offline_update_engine test_offline_update_helper
Pop-Location
```

本次从待提交的 Git 索引独立导出源码验证：上述回归加开发源码筛选测试共 **235 项通过**，
该独立副本的 Keil 应用构建 **0 错误、0 警告**。
正向跳转诊断单元测试使用明确标注的合成小数据，不要求上传现场私有采集记录。

Qt 单元测试使用离屏界面；不要直接运行 `capture`、`calibrate`、`flash`、`validate_*_live` 来代替回归。
这些开发工具可能开光、烧录或驱动机械臂；只有明确确认目标和安全条件后才能使用。

固件需 Keil MDK-ARM 5 / ARM Compiler 6，应用工程为 `JDSU/MDK-ARM/JDSU.uvprojx`。
当前加载地址为 `0x08008000`，应用区 224 KiB，完整 Flash 为 512 KiB；不要用旧的零地址应用覆盖引导区。
具体布局见 `JDSU/Core/Inc/ota_layout.h`，不可省略配套散装加载文件。
全机共用温控和正式精简配置要求见 [固件开发要求](JDSU/AGENTS.md)。

`firmware_features.h` 默认关闭旧应力/温度寻峰、MQTT 和网络 OTA；
USB 固件更新、LAN 数据、共享 EXTRA 采集及故障保护保留。
尤其必须保留 `01/08 enable=0 → 02/05` 的精确安全停止 ACK：v1.0.107 的精简回归曾破坏它，v1.0.108 已修复。
候选固件编译通过后仍须对应设备验收，不能直接升入客户更新目录。

## 客户更新与保密

当前采用 **开发者发送完整签名更新包，客户运行独立更新助手** 的方式。
主软件不自动检查 GitHub，不显示两个更新页/气泡；客户端更新和 USB 固件更新是助手内两个独立操作。
客户安装版和助手不需要 Python；USB 升级要求设备已有兼容引导程序，不能代替首次烧录。
可按客户选择发送，也可统一发送；这不是在线推送/授权服务。

发布流程见 [`deployment/offline_update_workflow.txt`](deployment/offline_update_workflow.txt)。
清单使用 Ed25519 签名，载荷使用 SHA-256；`Python/update_public_key.py` 仅含公钥。
私钥由发布者保管，不能提交 Git、装进客户端或交给客户。重新生成密钥不能自动被已有助手信任。
固件 UID 白名单用于隔离刷写对象，并不阻止客户端包转发，也不提供源码/固件加密。

Wi-Fi/OTA/MQTT 凭据、DPAPI 密文、`.release-signing/`、`*_PRIVATE/` 及现场设备配置禁止上传。
只保留配置 `.example`，运行环境由开发者自行配置。
历史 [GitHub Releases 更新说明](deployment/GITHUB_RELEASES_UPDATES.md) 作为研发参考保留，不是当前客户默认流程。

此仓库是开发协作源码，不是客户安装包。提交到 GitHub 不会自动修改连接的板卡，也不会自动给客户升级。
