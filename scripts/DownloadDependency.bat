@echo off
pushd %~dp0\..\
call python.exe DependencyDownloader.py
popd
PAUSE