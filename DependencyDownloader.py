import os
import sys
import requests
import subprocess

# 检查是否安装 PyGithub
try:
    import github
    print("PyGithub 已安装")
except ImportError:
    print("PyGithub 未安装，正在安装...")
    subprocess.check_call([sys.executable, '-m', 'pip', 'install', 'PyGithub'])
    print("PyGithub 安装完成")

from github import Github

# 使用 Personal Access Token 或匿名访问
g = Github()  # 可以替换为 Github("your_github_token")

repo_name = "Acmen-Team/Dark-Tools"  # 替换为你想查询的仓库
download_directory = "Test/downloads"  # 指定下载目录

# 创建下载目录（如果不存在）
os.makedirs(download_directory, exist_ok=True)

# 获取仓库
repo = g.get_repo(repo_name)

# 获取所有 release
releases = repo.get_releases()

# 列出所有的 release 及其资产
for release in releases:
    print(f"Release: {release.title}")
    print(f"Tag: {release.tag_name}")
    print(f"Published at: {release.published_at}")

    # 获取并下载所有资产
    assets = release.get_assets()
    if assets.totalCount > 0:
        print("Assets:")
        for asset in assets:
            print(f"  - Name: {asset.name}")
            print(f"    Download URL: {asset.browser_download_url}")

            # 下载资产并显示进度
            response = requests.get(asset.browser_download_url, stream=True)
            if response.status_code == 200:
                total_size = int(response.headers.get('content-length', 0))
                downloaded_size = 0
                file_path = os.path.join(download_directory, asset.name)

                with open(file_path, 'wb') as f:
                    for data in response.iter_content(chunk_size=1024):
                        f.write(data)
                        downloaded_size += len(data)
                        # 计算进度
                        percent = (downloaded_size / total_size) * 100 if total_size else 0
                        print(f"\r    Downloading... {percent:.2f}% complete", end="")
                
                print(f"\n    Successfully downloaded to: {file_path}")
            else:
                print(f"    Failed to download: {response.status_code}")
    else:
        print("  No assets available.")
    
    print()  # 打印空行以分隔不同的 release
