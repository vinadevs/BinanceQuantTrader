param(
    [string]$vcpkg_root_directory,
    [string]$source_directory
)

function generate_grpc_message_files {
    param (
        [string]$protoc_executable_path,
        [string]$grpc_plugin_executable_path,
        [string]$proto_file_path,
        [string]$output_directory
    )

    # Ensure output directory exists
    if (-not (Test-Path -Path $output_directory)) {
        New-Item -ItemType Directory -Path $output_directory | Out-Null
    }

    $proto_directory = $proto_file_path | Split-Path -Parent

    # Run protoc command
    & $protoc_executable_path `
        --proto_path=$proto_directory `
        --cpp_out=$output_directory `
        --grpc_out=$output_directory `
        --plugin=protoc-gen-grpc=$grpc_plugin_executable_path `
        $proto_file_path

    if ($LASTEXITCODE -ne 0) {
        Write-Error "Protobuf generation failed for $proto_file_path."
        exit $LASTEXITCODE
    }

    Write-Host "Protobuf files successfully generated for $proto_file_path in $output_directory"
}

function add_precompiled_header_include {
    param(
        [string]$directory_path
    )

    $pch_header_include = "#include ""pch.h"""
    $proto_files = Get-ChildItem -Path $directory_path -Filter *.pb.cc

    foreach ($file in $proto_files) {
        $file_content = Get-Content -Path $file.FullName
        $updated_content = @()

        $header_added = $false
        foreach ($line in $file_content) {
            if (-not $header_added -and $line -match ".pb.h") {
                $updated_content += $pch_header_include
                $updated_content += ""
                $header_added = $true
            }

            $updated_content += $line
        }

        Set-Content -Path $file.FullName -Value $updated_content
    }
}

function add_copyright_notice {
    param(
        [string]$directory_path
    )

$notice = "/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/"

    $proto_files = Get-ChildItem -Path $directory_path -Filter *.pb.h

    foreach ($file in $proto_files) {
        $file_content = Get-Content -Path $file.FullName
        $updated_content = @()
        $updated_content += $notice
        $updated_content += ""

         foreach ($line in $file_content) {
            $updated_content += $line
        }

        Set-Content -Path $file.FullName -Value $updated_content
    }
}

function convert_line_endings_to_windows_format {
    param(
        [string]$directory_path
    )

    $proto_files = Get-ChildItem -Path $directory_path -Filter *.pb.cc
    $header_files = Get-ChildItem -Path $directory_path -Filter *.pb.h

    $all_files = $proto_files + $header_files
    foreach ($file in $all_files) {
        (Get-Content -Path $file.FullName) | ForEach-Object { $_ -replace "`n", "`r`n" } | Set-Content -Path $file.FullName
    }
}

function main {
    Write-Host "Generating protobuf files..."
    Write-Host "vcpkg_root_directory: $vcpkg_root_directory"
    Write-Host "source_directory: $source_directory"

    $proto_to_output_directory_map = @{
        "$source_directory\Source\GrpcProtobufMessage\user_account_data.proto" = @(
            "$source_directory\Source\ExchangeSimulator\protobuf",
            "$source_directory\Source\ExchangeConnectivity\protobuf"
        )
    }

    # Define paths for protoc executable and grpc plugin
    $protoc_executable_path = "$source_directory\$vcpkg_root_directory\x64-windows\tools\protobuf\protoc.exe"
    $grpc_plugin_executable_path = "$source_directory\$vcpkg_root_directory\x64-windows\tools\grpc\grpc_cpp_plugin.exe"

    foreach ($proto_file_path in $proto_to_output_directory_map.Keys) {
        $output_directories = $proto_to_output_directory_map[$proto_file_path]
        foreach ($output_directory in $output_directories) {
            Write-Host "Generating protobuf file..."
            Write-Host "proto_file_path: $proto_file_path"
            Write-Host "output_directory: $output_directory"
            generate_grpc_message_files `
                -protoc_executable_path $protoc_executable_path `
                -grpc_plugin_executable_path $grpc_plugin_executable_path `
                -proto_file_path $proto_file_path `
                -output_directory $output_directory

            add_precompiled_header_include -directory_path $output_directory
            add_copyright_notice -directory_path $output_directory
            convert_line_endings_to_windows_format -directory_path $output_directory
        }
    }
}

main
