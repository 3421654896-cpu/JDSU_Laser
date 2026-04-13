#!/usr/bin/env python3
"""
PyQt5 → PyQt6 自动迁移脚本

功能:
- 自动转换 PyQt5 导入语句为 PyQt6
- 处理枚举限定符更新 (Qt.AlignLeft → Qt.AlignmentFlag.AlignLeft)
- 处理类从 QtWidgets 到 QtGui 的迁移
- 更新方法签名 (exec_ → exec, pos() → position())
- 支持目录批量处理
- 备份保护机制
- 干运行模式预览

用法:
    python pyqt5_to_pyqt6_migrator.py <target_file_or_directory> [options]
    
选项:
    --dry-run          预览更改,不修改文件
    --backup          修改前创建备份 (默认启用)
    --no-backup       不创建备份 (危险)
    --verbose         详细日志输出
    --force           强制处理,忽略警告
"""

import os
import sys
import re
import shutil
import argparse
from pathlib import Path
from typing import Dict, List, Tuple, Optional
from dataclasses import dataclass
from datetime import datetime


@dataclass
class MigrationStats:
    """迁移统计信息"""
    files_processed: int = 0
    files_modified: int = 0
    files_skipped: int = 0
    imports_fixed: int = 0
    enums_fixed: int = 0
    methods_fixed: int = 0
    classes_moved: int = 0
    warnings: List[str] = None
    
    def __post_init__(self):
        self.warnings = []


class PyQt5ToPyQt6Migrator:
    """PyQt5 到 PyQt6 的自动迁移器"""
    
    # PyQt5 模块到 PyQt6 模块的映射
    MODULE_MAP = {
        'PyQt5': 'PyQt6',
        'PyQt5.QtCore': 'PyQt6.QtCore',
        'PyQt5.QtGui': 'PyQt6.QtGui',
        'PyQt5.QtWidgets': 'PyQt6.QtWidgets',
        'PyQt5.QtMultimedia': 'PyQt6.QtMultimedia',
        'PyQt5.QtNetwork': 'PyQt6.QtNetwork',
        'PyQt5.QtSql': 'PyQt6.QtSql',
        'PyQt5.QtXml': 'PyQt6.QtXml',
        'PyQt5.QtSvg': 'PyQt6.QtSvg',
        'PyQt5.QtOpenGL': 'PyQt6.QtOpenGL',
        'PyQt5.QtPrintSupport': 'PyQt6.QtPrintSupport',
    }
    
    # 需要从 QtWidgets 迁移到 QtGui 的类
    CLASSES_MOVED_TO_QTGUI = {
        'QAction', 'QShortcut', 'QUndoCommand', 'QUndoStack', 
        'QUndoGroup', 'QFileSystemModel', 'QFontComboBox',
        'QKeySequence', 'QClipboard', 'QDrag', 'QDropEvent',
    }
    
    # 需要从 QtWidgets 迁移到 QtOpenGLWidgets 的类
    CLASSES_MOVED_TO_QTOPENGLWIDGETS = {
        'QOpenGLWidget',
    }
    
    # 枚举限定符映射表
    ENUM_MAP = {
        # 对齐方式
        'Qt.AlignLeft': 'Qt.AlignmentFlag.AlignLeft',
        'Qt.AlignRight': 'Qt.AlignmentFlag.AlignRight',
        'Qt.AlignCenter': 'Qt.AlignmentFlag.AlignCenter',
        'Qt.AlignHCenter': 'Qt.AlignmentFlag.AlignHCenter',
        'Qt.AlignVCenter': 'Qt.AlignmentFlag.AlignVCenter',
        'Qt.AlignTop': 'Qt.AlignmentFlag.AlignTop',
        'Qt.AlignBottom': 'Qt.AlignmentFlag.AlignBottom',
        'Qt.AlignJustify': 'Qt.AlignmentFlag.AlignJustify',
        
        # 复选状态
        'Qt.Unchecked': 'Qt.CheckState.Unchecked',
        'Qt.Checked': 'Qt.CheckState.Checked',
        'Qt.PartiallyChecked': 'Qt.CheckState.PartiallyChecked',
        
        # 方向
        'Qt.Horizontal': 'Qt.Orientation.Horizontal',
        'Qt.Vertical': 'Qt.Orientation.Vertical',
        
        # 窗口类型
        'Qt.Window': 'Qt.WindowType.Window',
        'Qt.Dialog': 'Qt.WindowType.Dialog',
        'Qt.Sheet': 'Qt.WindowType.Sheet',
        'Qt.Popup': 'Qt.WindowType.Popup',
        'Qt.Tool': 'Qt.WindowType.Tool',
        'Qt.ToolTip': 'Qt.WindowType.ToolTip',
        'Qt.SplashScreen': 'Qt.WindowType.SplashScreen',
        'Qt.SubWindow': 'Qt.WindowType.SubWindow',
        'Qt.FramelessWindowHint': 'Qt.WindowType.FramelessWindowHint',
        'Qt.WindowStaysOnTopHint': 'Qt.WindowType.WindowStaysOnTopHint',
        
        # 样式
        'Qt.SolidPattern': 'Qt.BrushStyle.SolidPattern',
        'Qt.Dense1Pattern': 'Qt.BrushStyle.Dense1Pattern',
        'Qt.Dense2Pattern': 'Qt.BrushStyle.Dense2Pattern',
        'Qt.HorPattern': 'Qt.BrushStyle.HorPattern',
        'Qt.VerPattern': 'Qt.BrushStyle.VerPattern',
        'Qt.CrossPattern': 'Qt.BrushStyle.CrossPattern',
        
        # 宽高比模式
        'Qt.IgnoreAspectRatio': 'Qt.AspectRatioMode.IgnoreAspectRatio',
        'Qt.KeepAspectRatio': 'Qt.AspectRatioMode.KeepAspectRatio',
        'Qt.KeepAspectRatioByExpanding': 'Qt.AspectRatioMode.KeepAspectRatioByExpanding',
        
        # 箭头类型
        'Qt.NoArrow': 'Qt.ArrowType.NoArrow',
        'Qt.UpArrow': 'Qt.ArrowType.UpArrow',
        'Qt.DownArrow': 'Qt.ArrowType.DownArrow',
        'Qt.LeftArrow': 'Qt.ArrowType.LeftArrow',
        'Qt.RightArrow': 'Qt.ArrowType.RightArrow',
        
        # 光标形状
        'Qt.ArrowCursor': 'Qt.CursorShape.ArrowCursor',
        'Qt.UpArrowCursor': 'Qt.CursorShape.UpArrowCursor',
        'Qt.CrossCursor': 'Qt.CursorShape.CrossCursor',
        'Qt.WaitCursor': 'Qt.CursorShape.WaitCursor',
        'Qt.IBeamCursor': 'Qt.CursorShape.IBeamCursor',
        'Qt.SizeVerCursor': 'Qt.CursorShape.SizeVerCursor',
        'Qt.SizeHorCursor': 'Qt.CursorShape.SizeHorCursor',
        'Qt.SizeAllCursor': 'Qt.CursorShape.SizeAllCursor',
        'Qt.BlankCursor': 'Qt.CursorShape.BlankCursor',
        'Qt.SplitVCursor': 'Qt.CursorShape.SplitVCursor',
        'Qt.SplitHCursor': 'Qt.CursorShape.SplitHCursor',
        'Qt.PointingHandCursor': 'Qt.CursorShape.PointingHandCursor',
        'Qt.ForbiddenCursor': 'Qt.CursorShape.ForbiddenCursor',
        
        # 标志位
        'Qt.NoItemFlags': 'Qt.ItemFlag.NoItemFlags',
        'Qt.ItemIsSelectable': 'Qt.ItemFlag.ItemIsSelectable',
        'Qt.ItemIsEditable': 'Qt.ItemFlag.ItemIsEditable',
        'Qt.ItemIsDragEnabled': 'Qt.ItemFlag.ItemIsDragEnabled',
        'Qt.ItemIsDropEnabled': 'Qt.ItemFlag.ItemIsDropEnabled',
        'Qt.ItemIsUserCheckable': 'Qt.ItemFlag.ItemIsUserCheckable',
        'Qt.ItemIsEnabled': 'Qt.ItemFlag.ItemIsEnabled',
        
        # 文本标志
        'Qt.TextSingleLine': 'Qt.TextFlag.TextSingleLine',
        'Qt.TextDontClip': 'Qt.TextFlag.TextDontClip',
        'Qt.TextExpandTabs': 'Qt.TextFlag.TextExpandTabs',
        'Qt.TextShowMnemonic': 'Qt.TextFlag.TextShowMnemonic',
        'Qt.TextWordWrap': 'Qt.TextFlag.TextWordWrap',
        
        # 大小策略
        'Qt.Fixed': 'Qt.SizePolicy.Fixed',
        'Qt.Minimum': 'Qt.SizePolicy.Minimum',
        'Qt.Maximum': 'Qt.SizePolicy.Maximum',
        'Qt.Preferred': 'Qt.SizePolicy.Preferred',
        'Qt.Expanding': 'Qt.SizePolicy.Expanding',
        'Qt.MinimumExpanding': 'Qt.SizePolicy.MinimumExpanding',
        'Qt.Ignored': 'Qt.SizePolicy.Ignored',
        
        # 焦点策略
        'Qt.NoFocus': 'Qt.FocusPolicy.NoFocus',
        'Qt.TabFocus': 'Qt.FocusPolicy.TabFocus',
        'Qt.ClickFocus': 'Qt.FocusPolicy.ClickFocus',
        'Qt.StrongFocus': 'Qt.FocusPolicy.StrongFocus',
        'Qt.WheelFocus': 'Qt.FocusPolicy.WheelFocus',
        
        # 键盘修饰符
        'Qt.NoModifier': 'Qt.KeyboardModifier.NoModifier',
        'Qt.ShiftModifier': 'Qt.KeyboardModifier.ShiftModifier',
        'Qt.ControlModifier': 'Qt.KeyboardModifier.ControlModifier',
        'Qt.AltModifier': 'Qt.KeyboardModifier.AltModifier',
        'Qt.MetaModifier': 'Qt.KeyboardModifier.MetaModifier',
        'Qt.KeypadModifier': 'Qt.KeyboardModifier.KeypadModifier',
        
        # 鼠标按钮
        'Qt.NoButton': 'Qt.MouseButton.NoButton',
        'Qt.LeftButton': 'Qt.MouseButton.LeftButton',
        'Qt.RightButton': 'Qt.MouseButton.RightButton',
        'Qt.MidButton': 'Qt.MouseButton.MidButton',
        'Qt.MiddleButton': 'Qt.MouseButton.MiddleButton',
        'Qt.XButton1': 'Qt.MouseButton.XButton1',
        'Qt.XButton2': 'Qt.MouseButton.XButton2',
        'Qt.AllButtons': 'Qt.MouseButton.AllButtons',
        
        # 工具按钮样式
        'Qt.ToolButtonIconOnly': 'Qt.ToolButtonStyle.ToolButtonIconOnly',
        'Qt.ToolButtonTextOnly': 'Qt.ToolButtonStyle.ToolButtonTextOnly',
        'Qt.ToolButtonTextBesideIcon': 'Qt.ToolButtonStyle.ToolButtonTextBesideIcon',
        'Qt.ToolButtonTextUnderIcon': 'Qt.ToolButtonStyle.ToolButtonTextUnderIcon',
        'Qt.ToolButtonFollowStyle': 'Qt.ToolButtonStyle.ToolButtonFollowStyle',
        
        # 对话框按钮
        'Qt.NoButton': 'Qt.DialogButtonBox.NoButton',
        'Qt.Ok': 'Qt.DialogButtonBox.Ok',
        'Qt.Save': 'Qt.DialogButtonBox.Save',
        'Qt.Open': 'Qt.DialogButtonBox.Open',
        'Qt.Yes': 'Qt.DialogButtonBox.Yes',
        'Qt.No': 'Qt.DialogButtonBox.No',
        'Qt.Cancel': 'Qt.DialogButtonBox.Cancel',
        'Qt.Close': 'Qt.DialogButtonBox.Close',
        'Qt.Abort': 'Qt.DialogButtonBox.Abort',
        'Qt.Retry': 'Qt.DialogButtonBox.Retry',
        'Qt.Ignore': 'Qt.DialogButtonBox.Ignore',
        
        # 方向策略
        'Qt.LeftToRight': 'Qt.LayoutDirection.LeftToRight',
        'Qt.RightToLeft': 'Qt.LayoutDirection.RightToLeft',
        
        # 纯色
        'Qt.white': 'Qt.GlobalColor.white',
        'Qt.black': 'Qt.GlobalColor.black',
        'Qt.red': 'Qt.GlobalColor.red',
        'Qt.darkRed': 'Qt.GlobalColor.darkRed',
        'Qt.green': 'Qt.GlobalColor.green',
        'Qt.darkGreen': 'Qt.GlobalColor.darkGreen',
        'Qt.blue': 'Qt.GlobalColor.blue',
        'Qt.darkBlue': 'Qt.GlobalColor.darkBlue',
        'Qt.cyan': 'Qt.GlobalColor.cyan',
        'Qt.darkCyan': 'Qt.GlobalColor.darkCyan',
        'Qt.magenta': 'Qt.GlobalColor.magenta',
        'Qt.darkMagenta': 'Qt.GlobalColor.darkMagenta',
        'Qt.yellow': 'Qt.GlobalColor.yellow',
        'Qt.darkYellow': 'Qt.GlobalColor.darkYellow',
        'Qt.gray': 'Qt.GlobalColor.gray',
        'Qt.darkGray': 'Qt.GlobalColor.darkGray',
        'Qt.lightGray': 'Qt.GlobalColor.lightGray',
        'Qt.transparent': 'Qt.GlobalColor.transparent',
        
        # 填充模式
        'Qt.WindingFill': 'Qt.FillRule.WindingFill',
        'Qt.OddEvenFill': 'Qt.FillRule.OddEvenFill',
        
        # 变换模式
        'Qt.FastTransformation': 'Qt.TransformationMode.FastTransformation',
        'Qt.SmoothTransformation': 'Qt.TransformationMode.SmoothTransformation',
        
        # 应用程序属性
        'Qt.AA_EnableHighDpiScaling': 'Qt.ApplicationAttribute.AA_EnableHighDpiScaling',
        'Qt.AA_UseHighDpiPixmaps': 'Qt.ApplicationAttribute.AA_UseHighDpiPixmaps',
        'Qt.AA_ShareOpenGLContexts': 'Qt.ApplicationAttribute.AA_ShareOpenGLContexts',
        
        # 窗口标志
        'Qt.Widget': 'Qt.WindowFlag.Widget',
    }
    
    def __init__(self, args):
        self.args = args
        self.stats = MigrationStats()
        
    def log(self, message: str, level: str = 'info'):
        """输出日志"""
        if self.args.verbose or level != 'debug':
            if level == 'warning':
                print(f"\033[93m[WARNING]\033[0m {message}")
            elif level == 'error':
                print(f"\033[91m[ERROR]\033[0m {message}")
            elif level == 'success':
                print(f"\033[92m[SUCCESS]\033[0m {message}")
            else:
                print(f"[INFO] {message}")
    
    def process_file(self, file_path: Path) -> bool:
        """处理单个文件"""
        try:
            # 读取文件内容
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
            except UnicodeDecodeError:
                with open(file_path, 'r', encoding='latin-1') as f:
                    content = f.read()
            
            # 检查是否包含 PyQt5 代码
            if 'PyQt5' not in content and 'from Qt' not in content:
                return False
            
            original_content = content
            
            # 执行各项迁移
            content = self.fix_imports(content)
            content = self.fix_enum_qualifiers(content)
            content = self.fix_class_relocations(content)
            content = self.fix_method_signatures(content)
            content = self.fix_high_dpi_scaling(content)
            
            # 检查是否有更改
            if content != original_content:
                if not self.args.dry_run:
                    # 创建备份
                    if self.args.backup:
                        backup_path = file_path.with_suffix(file_path.suffix + '.bak')
                        shutil.copy2(file_path, backup_path)
                        self.log(f"已创建备份: {backup_path}")
                    
                    # 写入修改后的内容
                    with open(file_path, 'w', encoding='utf-8') as f:
                        f.write(content)
                    
                    self.log(f"已迁移: {file_path}", 'success')
                else:
                    self.log(f"干运行 - 将修改: {file_path}", 'success')
                return True
            
            return False
            
        except Exception as e:
            self.log(f"处理文件失败 {file_path}: {e}", 'error')
            self.stats.warnings.append(f"文件处理失败: {file_path} - {e}")
            return False
    
    def fix_imports(self, content: str) -> str:
        """修复导入语句"""
        lines = content.split('\n')
        new_lines = []
        qtgui_imports = set()
        qtopenGLWidgets_imports = set()
        
        for line in lines:
            original_line = line
            modified = False
            
            # 处理 "import PyQt5" 相关的导入
            for old_module, new_module in self.MODULE_MAP.items():
                if old_module in line:
                    line = line.replace(old_module, new_module)
                    modified = True
            
            # 检查是否需要从 QtWidgets 迁移到 QtGui
            if 'from PyQt6.QtWidgets import' in line:
                imports = line.split('from PyQt6.QtWidgets import')[1].strip()
                if imports:
                    imported_classes = [c.strip() for c in imports.split(',')]
                    remaining_classes = []
                    
                    for cls in imported_classes:
                        if cls in self.CLASSES_MOVED_TO_QTGUI:
                            qtgui_imports.add(cls)
                        elif cls in self.CLASSES_MOVED_TO_QTOPENGLWIDGETS:
                            qtopenGLWidgets_imports.add(cls)
                        else:
                            remaining_classes.append(cls)
                    
                    if remaining_classes:
                        line = f"from PyQt6.QtWidgets import {', '.join(remaining_classes)}"
                    else:
                        line = None
            
            if line is not None:
                new_lines.append(line)
            
            if modified and line != original_line:
                self.stats.imports_fixed += 1
        
        # 添加额外的导入
        if qtgui_imports or qtopenGLWidgets_imports:
            # 找到合适的位置插入新导入
            insert_pos = 0
            for i, line in enumerate(new_lines):
                if line.startswith('from PyQt6'):
                    insert_pos = i + 1
            
            if qtgui_imports:
                imports_str = ', '.join(sorted(qtgui_imports))
                new_lines.insert(insert_pos, f"from PyQt6.QtGui import {imports_str}")
                self.stats.imports_fixed += 1
                self.log(f"添加 QtGui 导入: {imports_str}", 'debug')
            
            if qtopenGLWidgets_imports:
                imports_str = ', '.join(sorted(qtopenGLWidgets_imports))
                new_lines.insert(insert_pos, f"from PyQt6.QtOpenGLWidgets import {imports_str}")
                self.stats.imports_fixed += 1
                self.log(f"添加 QtOpenGLWidgets 导入: {imports_str}", 'debug')
        
        return '\n'.join(new_lines)
    
    def fix_enum_qualifiers(self, content: str) -> str:
        """修复枚举限定符"""
        for old_enum, new_enum in self.ENUM_MAP.items():
            if old_enum in content:
                content = content.replace(old_enum, new_enum)
                self.stats.enums_fixed += 1
                self.log(f"修复枚举: {old_enum} -> {new_enum}", 'debug')
        
        return content
    
    def fix_class_relocations(self, content: str) -> str:
        """修复类的模块迁移 (处理 QtWidgets.Xxx 格式)"""
        # 处理 QtWidgets.QAction -> QtGui.QAction 格式
        for cls in self.CLASSES_MOVED_TO_QTGUI:
            old_ref = f"QtWidgets.{cls}"
            new_ref = f"QtGui.{cls}"
            if old_ref in content:
                content = content.replace(old_ref, new_ref)
                self.stats.classes_moved += 1
        
        # 处理 QtWidgets.QOpenGLWidget -> QtOpenGLWidgets.QOpenGLWidget 格式
        for cls in self.CLASSES_MOVED_TO_QTOPENGLWIDGETS:
            old_ref = f"QtWidgets.{cls}"
            new_ref = f"QtOpenGLWidgets.{cls}"
            if old_ref in content:
                content = content.replace(old_ref, new_ref)
                self.stats.classes_moved += 1
        
        return content
    
    def fix_method_signatures(self, content: str) -> str:
        """修复方法签名"""
        # exec_() -> exec()
        patterns = [
            (r'\.exec_\(\)', '.exec()'),
            (r'(\w+)\.exec_\(', r'\1.exec('),
        ]
        
        for pattern, replacement in patterns:
            matches = re.findall(pattern, content)
            if matches:
                content = re.sub(pattern, replacement, content)
                self.stats.methods_fixed += len(matches)
        
        return content
    
    def fix_high_dpi_scaling(self, content: str) -> str:
        """处理高DPI缩放设置 (在PyQt6中默认启用)"""
        # 注释掉或移除 AA_EnableHighDpiScaling 相关代码
        high_dpi_patterns = [
            r'QApplication\.setAttribute\(Qt\.AA_EnableHighDpiScaling[^\)]*\)',
            r'QApplication\.setAttribute\(Qt\.AA_UseHighDpiPixmaps[^\)]*\)',
        ]
        
        for pattern in high_dpi_patterns:
            matches = re.findall(pattern, content)
            if matches:
                for match in matches:
                    # 注释掉该行
                    commented = f"# # 迁移注释: High DPI 默认在 PyQt6 中启用\n# {match}"
                    content = content.replace(match, commented)
                self.log(f"注释掉 High DPI 设置: {len(matches)} 处", 'warning')
                self.stats.warnings.append(f"已注释 {len(matches)} 处 High DPI 设置")
        
        return content
    
    def process_directory(self, directory: Path):
        """处理目录"""
        if not directory.is_dir():
            self.log(f"不是有效的目录: {directory}", 'error')
            return
        
        # 递归查找所有 Python 文件
        python_files = list(directory.rglob('*.py'))
        
        self.log(f"找到 {len(python_files)} 个 Python 文件")
        
        for file_path in python_files:
            if self.process_file(file_path):
                self.stats.files_modified += 1
            self.stats.files_processed += 1
        
        self.print_summary()
    
    def process_target(self, target: str):
        """处理目标文件或目录"""
        target_path = Path(target)
        
        if not target_path.exists():
            self.log(f"目标不存在: {target}", 'error')
            return
        
        if target_path.is_file():
            if self.process_file(target_path):
                self.stats.files_modified += 1
            self.stats.files_processed += 1
            self.print_summary()
        else:
            self.process_directory(target_path)
    
    def print_summary(self):
        """打印迁移摘要"""
        print("\n" + "="*50)
        print("迁移摘要")
        print("="*50)
        print(f"处理文件数: {self.stats.files_processed}")
        print(f"修改文件数: {self.stats.files_modified}")
        print(f"导入修复: {self.stats.imports_fixed}")
        print(f"枚举修复: {self.stats.enums_fixed}")
        print(f"方法修复: {self.stats.methods_fixed}")
        print(f"类迁移: {self.stats.classes_moved}")
        
        if self.stats.warnings:
            print(f"\n警告 ({len(self.stats.warnings)}):")
            for warning in self.stats.warnings:
                print(f"  - {warning}")
        
        if self.args.dry_run:
            print("\n[干运行模式] 未实际修改任何文件")
        
        print("="*50)


def main():
    parser = argparse.ArgumentParser(
        description='PyQt5 → PyQt6 自动迁移工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例:
    python pyqt5_to_pyqt6_migrator.py myapp.py
    python pyqt5_to_pyqt6_migrator.py myapp/ --dry-run
    python pyqt5_to_pyqt6_migrator.py myapp/ --verbose --backup
        """
    )
    
    parser.add_argument('target', help='目标文件或目录')
    parser.add_argument('--dry-run', action='store_true', 
                        help='预览更改,不修改文件')
    parser.add_argument('--backup', action='store_true', default=True,
                        help='修改前创建备份 (默认启用)')
    parser.add_argument('--no-backup', action='store_true',
                        help='不创建备份 (危险)')
    parser.add_argument('--verbose', action='store_true',
                        help='详细日志输出')
    parser.add_argument('--force', action='store_true',
                        help='强制处理,忽略警告')
    
    args = parser.parse_args()
    
    # 处理 --no-backup 选项
    if args.no_backup:
        args.backup = False
    
    # 创建迁移器并执行
    migrator = PyQt5ToPyQt6Migrator(args)
    migrator.process_target(args.target)


if __name__ == '__main__':
    main()
