/*******************************************************************************
Copyright 2018 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "emulated_disk.h"

#ifdef SECTOR_SIZE
#error "Sector size already defined."
#endif

#define SECTOR_SIZE 512

static const uint8_t sector0[] = { 0x33, 0xC0, 0xFA, 0x8E, 0xD8, 0x8E, 0xD0, 0xBC, 0x00, 0x7C, 0x89, 0xE6, 0x06, 0x57, 0x8E, 0xC0, 0xFB, 0xFC, 0xBF, 0x00, 0x06, 0xB9, 0x00, 0x01, 0xF3, 0xA5, 0xEA, 0x1F, 0x06, 0x00, 0x00, 0x52, 0x52, 0xB4, 0x41, 0xBB, 0xAA, 0x55, 0x31, 0xC9, 0x30, 0xF6, 0xF9, 0xCD, 0x13, 0x72, 0x13, 0x81, 0xFB, 0x55, 0xAA, 0x75, 0x0D, 0xD1, 0xE9, 0x73, 0x09, 0x66, 0xC7, 0x06, 0x8D, 0x06, 0xB4, 0x42, 0xEB, 0x15, 0x5A, 0xB4, 0x08, 0xCD, 0x13, 0x83, 0xE1, 0x3F, 0x51, 0x0F, 0xB6, 0xC6, 0x40, 0xF7, 0xE1, 0x52, 0x50, 0x66, 0x31, 0xC0, 0x66, 0x99, 0xE8, 0x66, 0x00, 0xE8, 0x21, 0x01, 0x4D, 0x69, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2E, 0x0D, 0x0A, 0x66, 0x60, 0x66, 0x31, 0xD2, 0xBB, 0x00, 0x7C, 0x66, 0x52, 0x66, 0x50, 0x06, 0x53, 0x6A, 0x01, 0x6A, 0x10, 0x89, 0xE6, 0x66, 0xF7, 0x36, 0xF4, 0x7B, 0xC0, 0xE4, 0x06, 0x88, 0xE1, 0x88, 0xC5, 0x92, 0xF6, 0x36, 0xF8, 0x7B, 0x88, 0xC6, 0x08, 0xE1, 0x41, 0xB8, 0x01, 0x02, 0x8A, 0x16, 0xFA, 0x7B, 0xCD, 0x13, 0x8D, 0x64, 0x10, 0x66, 0x61, 0xC3, 0xE8, 0xC4, 0xFF, 0xBE, 0xBE, 0x7D, 0xBF, 0xBE, 0x07, 0xB9, 0x20, 0x00, 0xF3, 0xA5, 0xC3, 0x66, 0x60, 0x89, 0xE5, 0xBB, 0xBE, 0x07, 0xB9, 0x04, 0x00, 0x31, 0xC0, 0x53, 0x51, 0xF6, 0x07, 0x80, 0x74, 0x03, 0x40, 0x89, 0xDE, 0x83, 0xC3, 0x10, 0xE2, 0xF3, 0x48, 0x74, 0x5B, 0x79, 0x39, 0x59, 0x5B, 0x8A, 0x47, 0x04, 0x3C, 0x0F, 0x74, 0x06, 0x24, 0x7F, 0x3C, 0x05, 0x75, 0x22, 0x66, 0x8B, 0x47, 0x08, 0x66, 0x8B, 0x56, 0x14, 0x66, 0x01, 0xD0, 0x66, 0x21, 0xD2, 0x75, 0x03, 0x66, 0x89, 0xC2, 0xE8, 0xAC, 0xFF, 0x72, 0x03, 0xE8, 0xB6, 0xFF, 0x66, 0x8B, 0x46, 0x1C, 0xE8, 0xA0, 0xFF, 0x83, 0xC3, 0x10, 0xE2, 0xCC, 0x66, 0x61, 0xC3, 0xE8, 0x62, 0x00, 0x4D, 0x75, 0x6C, 0x74, 0x69, 0x70, 0x6C, 0x65, 0x20, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x20, 0x70, 0x61, 0x72, 0x74, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x73, 0x2E, 0x0D, 0x0A, 0x66, 0x8B, 0x44, 0x08, 0x66, 0x03, 0x46, 0x1C, 0x66, 0x89, 0x44, 0x08, 0xE8, 0x30, 0xFF, 0x72, 0x13, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x0F, 0x85, 0x06, 0xFF, 0xBC, 0xFA, 0x7B, 0x5A, 0x5F, 0x07, 0xFA, 0xFF, 0xE4, 0xE8, 0x1E, 0x00, 0x4F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x20, 0x65, 0x72, 0x72, 0x6F, 0x72, 0x2E, 0x0D, 0x0A, 0x5E, 0xAC, 0xB4, 0x0E, 0x8A, 0x3E, 0x62, 0x04, 0xB3, 0x07, 0xCD, 0x10, 0x3C, 0x0A, 0x75, 0xF1, 0xCD, 0x18, 0xF4, 0xEB, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x02, 0x00, 0x0C, 0x1C, 0xDC, 0xF8, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x47, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector64[] = { 0xEB, 0x58, 0x90, 0x53, 0x59, 0x53, 0x4C, 0x49, 0x4E, 0x55, 0x58, 0x00, 0x02, 0x04, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3F, 0x00, 0xFF, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x47, 0x1F, 0x00, 0x95, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x29, 0x51, 0x1D, 0xEB, 0x16, 0x4E, 0x4F, 0x20, 0x4E, 0x41, 0x4D, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41, 0x54, 0x33, 0x32, 0x20, 0x20, 0x20, 0xFA, 0xFC, 0x31, 0xC9, 0x8E, 0xD1, 0xBC, 0x76, 0x7B, 0x52, 0x06, 0x57, 0x1E, 0x56, 0x8E, 0xC1, 0xB1, 0x26, 0xBF, 0x78, 0x7B, 0xF3, 0xA5, 0x8E, 0xD9, 0xBB, 0x78, 0x00, 0x0F, 0xB4, 0x37, 0x0F, 0xA0, 0x56, 0x20, 0xD2, 0x78, 0x1B, 0x31, 0xC0, 0xB1, 0x06, 0x89, 0x3F, 0x89, 0x47, 0x02, 0xF3, 0x64, 0xA5, 0x8A, 0x0E, 0x18, 0x7C, 0x88, 0x4D, 0xF8, 0x50, 0x50, 0x50, 0x50, 0xCD, 0x13, 0xEB, 0x62, 0x8B, 0x55, 0xAA, 0x8B, 0x75, 0xA8, 0xC1, 0xEE, 0x04, 0x01, 0xF2, 0x83, 0xFA, 0x4F, 0x76, 0x31, 0x81, 0xFA, 0xB2, 0x07, 0x73, 0x2B, 0xF6, 0x45, 0xB4, 0x7F, 0x75, 0x25, 0x38, 0x4D, 0xB8, 0x74, 0x20, 0x66, 0x3D, 0x21, 0x47, 0x50, 0x54, 0x75, 0x10, 0x80, 0x7D, 0xB8, 0xED, 0x75, 0x0A, 0x66, 0xFF, 0x75, 0xEC, 0x66, 0xFF, 0x75, 0xE8, 0xEB, 0x0F, 0x51, 0x51, 0x66, 0xFF, 0x75, 0xBC, 0xEB, 0x07, 0x51, 0x51, 0x66, 0xFF, 0x36, 0x1C, 0x7C, 0xB4, 0x08, 0xE8, 0xE9, 0x00, 0x72, 0x13, 0x20, 0xE4, 0x75, 0x0F, 0xC1, 0xEA, 0x08, 0x42, 0x89, 0x16, 0x1A, 0x7C, 0x83, 0xE1, 0x3F, 0x89, 0x0E, 0x18, 0x7C, 0xFB, 0xBB, 0xAA, 0x55, 0xB4, 0x41, 0xE8, 0xCB, 0x00, 0x72, 0x10, 0x81, 0xFB, 0x55, 0xAA, 0x75, 0x0A, 0xF6, 0xC1, 0x01, 0x74, 0x05, 0xC6, 0x06, 0x46, 0x7D, 0x00, 0x66, 0xB8, 0x4E, 0x1F, 0x00, 0x00, 0x66, 0xBA, 0x00, 0x00, 0x00, 0x00, 0xBB, 0x00, 0x80, 0xE8, 0x0E, 0x00, 0x66, 0x81, 0x3E, 0x1C, 0x80, 0xC5, 0xED, 0x5A, 0x70, 0x75, 0x74, 0xE9, 0xF8, 0x02, 0x66, 0x03, 0x06, 0x60, 0x7B, 0x66, 0x13, 0x16, 0x64, 0x7B, 0xB9, 0x10, 0x00, 0xEB, 0x2B, 0x66, 0x52, 0x66, 0x50, 0x06, 0x53, 0x6A, 0x01, 0x6A, 0x10, 0x89, 0xE6, 0x66, 0x60, 0xB4, 0x42, 0xE8, 0x77, 0x00, 0x66, 0x61, 0x8D, 0x64, 0x10, 0x72, 0x01, 0xC3, 0x66, 0x60, 0x31, 0xC0, 0xE8, 0x68, 0x00, 0x66, 0x61, 0xE2, 0xDA, 0xC6, 0x06, 0x46, 0x7D, 0x2B, 0x66, 0x60, 0x66, 0x0F, 0xB7, 0x36, 0x18, 0x7C, 0x66, 0x0F, 0xB7, 0x3E, 0x1A, 0x7C, 0x66, 0xF7, 0xF6, 0x31, 0xC9, 0x87, 0xCA, 0x66, 0xF7, 0xF7, 0x66, 0x3D, 0xFF, 0x03, 0x00, 0x00, 0x77, 0x17, 0xC0, 0xE4, 0x06, 0x41, 0x08, 0xE1, 0x88, 0xC5, 0x88, 0xD6, 0xB8, 0x01, 0x02, 0xE8, 0x2F, 0x00, 0x66, 0x61, 0x72, 0x01, 0xC3, 0xE2, 0xC9, 0x31, 0xF6, 0x8E, 0xD6, 0xBC, 0x68, 0x7B, 0x8E, 0xDE, 0x66, 0x8F, 0x06, 0x78, 0x00, 0xBE, 0xDA, 0x7D, 0xAC, 0x20, 0xC0, 0x74, 0x09, 0xB4, 0x0E, 0xBB, 0x07, 0x00, 0xCD, 0x10, 0xEB, 0xF2, 0x31, 0xC0, 0xCD, 0x16, 0xCD, 0x19, 0xF4, 0xEB, 0xFD, 0x8A, 0x16, 0x74, 0x7B, 0x06, 0xCD, 0x13, 0x07, 0xC3, 0x42, 0x6F, 0x6F, 0x74, 0x20, 0x65, 0x72, 0x72, 0x6F, 0x72, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x02, 0xB2, 0x3E, 0x18, 0x37, 0x55, 0xAA, };
static const uint8_t sector65[] = { 0x52, 0x52, 0x61, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x72, 0x41, 0x61, 0x09, 0xCA, 0x07, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector70[] = { 0xEB, 0x5A, 0x90, 0x4D, 0x53, 0x57, 0x49, 0x4E, 0x34, 0x2E, 0x31, 0x00, 0x02, 0x04, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3F, 0x00, 0xFF, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x47, 0x1F, 0x00, 0x95, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x29, 0x51, 0x1D, 0xEB, 0x16, 0x4E, 0x4F, 0x20, 0x4E, 0x41, 0x4D, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41, 0x54, 0x33, 0x32, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector71[] = { 0x52, 0x52, 0x61, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x72, 0x41, 0x61, 0x1C, 0xCA, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector96[] = { 0xF8, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const uint8_t sector4085[] = { 0xF8, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const uint8_t sector8074[] = { 0x4C, 0x44, 0x4C, 0x49, 0x4E, 0x55, 0x58, 0x20, 0x53, 0x59, 0x53, 0x27, 0x18, 0xB2, 0xEB, 0x79, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0xEC, 0x79, 0x2C, 0x41, 0x03, 0x00, 0x00, 0x90, 0x00, 0x00, 0x44, 0x52, 0x56, 0x30, 0x35, 0x39, 0x20, 0x20, 0x20, 0x20, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x88, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE5, 0x52, 0x49, 0x54, 0x45, 0x20, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x4A, 0x17, 0x00, 0x09, 0x00, 0x00, 0x00, 0xE5, 0x45, 0x4B, 0x30, 0x30, 0x30, 0x31, 0x20, 0x42, 0x4D, 0x50, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x45, 0x00, 0x00, 0x94, 0x74, 0x38, 0x45, 0x3C, 0x00, 0x36, 0x30, 0x01, 0x00, 0xE5, 0x45, 0x4B, 0x30, 0x30, 0x30, 0x32, 0x20, 0x42, 0x4D, 0x50, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x45, 0x00, 0x00, 0xA8, 0x74, 0x38, 0x45, 0x63, 0x00, 0x36, 0x30, 0x01, 0x00, 0xE5, 0x52, 0x00, 0x45, 0x00, 0x41, 0x00, 0x44, 0x00, 0x4D, 0x00, 0x0F, 0x00, 0xDE, 0x45, 0x00, 0x2E, 0x00, 0x64, 0x00, 0x69, 0x00, 0x73, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x64, 0x00, 0x65, 0x00, 0xE5, 0x45, 0x41, 0x44, 0x4D, 0x45, 0x7E, 0x31, 0x44, 0x49, 0x53, 0x20, 0x00, 0x93, 0xEC, 0x79, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x1A, 0x00, 0xE9, 0x00, 0x00, 0x00, 0xE5, 0x55, 0x54, 0x4F, 0x52, 0x55, 0x4E, 0x20, 0x49, 0x4E, 0x46, 0x20, 0x18, 0x95, 0xEC, 0x79, 0x2C, 0x41, 0x2C, 0x45, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x1B, 0x00, 0x86, 0x00, 0x00, 0x00, 0xE5, 0x4F, 0x4F, 0x54, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x97, 0xEC, 0x79, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x1C, 0x00, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x41, 0x53, 0x50, 0x45, 0x52, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x9D, 0xEC, 0x79, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x1F, 0x00, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x49, 0x53, 0x54, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x0D, 0xB7, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x91, 0x5A, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x4E, 0x53, 0x54, 0x41, 0x4C, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x74, 0xB7, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0xA2, 0x5A, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x59, 0x53, 0x4C, 0x49, 0x4E, 0x55, 0x58, 0x20, 0x20, 0x20, 0x10, 0x08, 0x36, 0xB8, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0xCB, 0x5D, 0x00, 0x18, 0x00, 0x00, 0xE5, 0x44, 0x35, 0x53, 0x55, 0x4D, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x18, 0xA7, 0xBD, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x25, 0x60, 0x46, 0x10, 0x00, 0x00, 0xE5, 0x49, 0x43, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0xB0, 0xBD, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x28, 0x60, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x4F, 0x4F, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x21, 0xC0, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0x38, 0x60, 0x00, 0x08, 0x00, 0x00, };
static const uint8_t sector8075[] = { 0xE5, 0x52, 0x45, 0x53, 0x45, 0x45, 0x44, 0x20, 0x20, 0x20, 0x20, 0x10, 0x08, 0x5A, 0xC3, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0xD2, 0x67, 0x00, 0x08, 0x00, 0x00, 0xE5, 0x55, 0x42, 0x49, 0x20, 0x20, 0x20, 0x20, 0x45, 0x58, 0x45, 0x20, 0x18, 0x71, 0xC3, 0x7A, 0x2C, 0x41, 0x2C, 0x45, 0x00, 0x00, 0x42, 0x92, 0x11, 0x41, 0xD6, 0x67, 0x78, 0x3C, 0x26, 0x00, 0xE5, 0x74, 0x00, 0x78, 0x00, 0x74, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x6C, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x72, 0x00, 0x2D, 0x00, 0x0F, 0x00, 0xA1, 0x43, 0x00, 0x6F, 0x00, 0x70, 0x00, 0x79, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x67, 0x00, 0x2E, 0x00, 0xE5, 0x55, 0x00, 0x6E, 0x00, 0x69, 0x00, 0x2D, 0x00, 0x55, 0x00, 0x0F, 0x00, 0xA1, 0x53, 0x00, 0x42, 0x00, 0x2D, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x73, 0x00, 0x00, 0x00, 0x74, 0x00, 0x61, 0x00, 0xE5, 0x4E, 0x49, 0x2D, 0x55, 0x53, 0x7E, 0x31, 0x54, 0x58, 0x54, 0x20, 0x00, 0x07, 0xC5, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0xCC, 0x88, 0x36, 0x40, 0x9F, 0x6C, 0xC0, 0xBF, 0x00, 0x00, 0xE5, 0x78, 0x00, 0x74, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x6C, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x72, 0x00, 0x2D, 0x00, 0x0F, 0x00, 0x81, 0x52, 0x00, 0x65, 0x00, 0x61, 0x00, 0x64, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x74, 0x00, 0xE5, 0x55, 0x00, 0x6E, 0x00, 0x69, 0x00, 0x2D, 0x00, 0x55, 0x00, 0x0F, 0x00, 0x81, 0x53, 0x00, 0x42, 0x00, 0x2D, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x73, 0x00, 0x00, 0x00, 0x74, 0x00, 0x61, 0x00, 0xE5, 0x4E, 0x49, 0x2D, 0x55, 0x53, 0x7E, 0x32, 0x54, 0x58, 0x54, 0x20, 0x00, 0xAC, 0xC5, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x3D, 0x82, 0x27, 0x41, 0xB7, 0x6C, 0x51, 0x34, 0x00, 0x00, 0xE5, 0x49, 0x43, 0x45, 0x4E, 0x53, 0x45, 0x20, 0x54, 0x58, 0x54, 0x20, 0x18, 0xC3, 0xC5, 0x7A, 0x2C, 0x41, 0x2C, 0x41, 0x00, 0x00, 0x53, 0x7D, 0x84, 0x40, 0xBE, 0x6C, 0xAC, 0x46, 0x00, 0x00, 0xE5, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x0F, 0x00, 0x0E, 0x2E, 0x00, 0x68, 0x00, 0x65, 0x00, 0x78, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x4D, 0x00, 0x50, 0x00, 0x4C, 0x00, 0x41, 0x00, 0x42, 0x00, 0x0F, 0x00, 0x0E, 0x2E, 0x00, 0x58, 0x00, 0x2E, 0x00, 0x70, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x64, 0x00, 0x75, 0x00, 0xE5, 0x50, 0x4C, 0x41, 0x42, 0x58, 0x7E, 0x31, 0x48, 0x45, 0x58, 0x20, 0x00, 0x10, 0x91, 0x61, 0x2C, 0x45, 0x2C, 0x45, 0x05, 0x00, 0x5D, 0x61, 0x2C, 0x45, 0x02, 0x00, 0xA4, 0xB6, 0x00, 0x00, 0xE5, 0x4D, 0x41, 0x47, 0x45, 0x20, 0x20, 0x20, 0x48, 0x45, 0x58, 0x20, 0x00, 0x10, 0x91, 0x61, 0x2C, 0x45, 0x2C, 0x45, 0x00, 0x00, 0x5D, 0x61, 0x2C, 0x45, 0x02, 0x00, 0xA4, 0xB6, 0x00, 0x00, 0x52, 0x45, 0x41, 0x44, 0x20, 0x20, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x00, 0xAC, 0x00, 0x54, 0x88, 0x4B, 0x88, 0x4B, 0x00, 0x00, 0xD0, 0x80, 0x87, 0x4B, 0x15, 0x00, 0x08, 0x00, 0x00, 0x00, };

static struct EMULATED_DISK disk = {0, NULL};

void DRV059_writeSector(const uint8_t *buff, uint32_t lba, size_t count)
{
    size_t i;
    
    for(i=0; i<count; i++){
        EmulatedDiskSectorWrite(&disk, (uint32_t)(lba + i), (uint8_t*)&buff[SECTOR_SIZE*i], true);
    }
    
    
}

void DRV059_readSector( uint8_t *buff, uint32_t lba, size_t count)
{
    size_t i;
    
    for(i=0; i<count; i++){
        EmulatedDiskSectorRead(&disk, (uint32_t)(lba + i), (uint8_t*)&buff[SECTOR_SIZE*i]);
    }
    
    
}

void DRV059_initialize(void)
{    
    EmulatedDiskCreate(&disk, SECTOR_SIZE);
    
    DRV059_writeSector((const uint8_t*)sector0, 0, 1);
    DRV059_writeSector((const uint8_t*)sector64, 64, 1);
    DRV059_writeSector((const uint8_t*)sector65, 65, 1);
    DRV059_writeSector((const uint8_t*)sector70, 70, 1);
    DRV059_writeSector((const uint8_t*)sector71, 71, 1);
    DRV059_writeSector((const uint8_t*)sector96, 96, 1);
    DRV059_writeSector((const uint8_t*)sector4085, 4085, 1);
    DRV059_writeSector((const uint8_t*)sector8074, 8074, 1);
    DRV059_writeSector((const uint8_t*)sector8075, 8075, 1);

}

void DRV059_print(void)
{
    EmulatedDiskPrint(&disk);
}

static const char id[] = "DRV059";
const char* DRV059_id(void)
{
    return id;
}

struct EMULATED_DRIVE DRV059 = { &DRV059_initialize, &DRV059_print, &DRV059_id};

