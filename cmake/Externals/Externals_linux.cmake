#////////////////////////////////////////////////////////////////////////////////////////////////////
#// Copyright (c) 2021 RacoonStudios
#//
#// Permission is hereby granted, free of charge, to any person obtaining a copy of this
#// software and associated documentation files (the "Software"), to deal in the Software
#// without restriction, including without limitation the rights to use, copy, modify, merge,
#// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
#// to whom the Software is furnished to do so, subject to the following conditions:
#//
#// The above copyright notice and this permission notice shall be included in all copies or
#// substantial portions of the Software.
#//
#// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
#// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
#// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#// DEALINGS IN THE SOFTWARE.
#////////////////////////////////////////////////////////////////////////////////////////////////////

set(DBUS_LIBRARIES "dbus-1")

pl_associate_package(
  PACKAGE_NAME lua-5.4.4-rev1-linux
  TARGETS lua
  PACKAGE_HASH 0ebd91ffee9baa8af92f4a166dcd400b7b7850935d75777e065f6247ded0de87
)
#pl_associate_package(
#  PACKAGE_NAME toml11-3.7.0-rev1-linux
#  TARGETS toml11
#  PACKAGE_HASH 1071a646c20f104e54dcab4d8f2ed1ccc32bc81cbb1aaeb6700bf358015aa081
#)
pl_associate_package(
  PACKAGE_NAME lz4-1.9.3-rev1-linux
  TARGETS lz4
  PACKAGE_HASH 902ee3879b4506d3544017a95441a1163537ab8c8886bc755891b93340f778d8
)
#pl_associate_package(
#  PACKAGE_NAME googletest-1.8.1-rev1-linux
#  TARGETS googletest
#  PACKAGE_HASH bccf2b2b956dd4726373c89ecdc44d2507db5587185b11e0cc7e05273b65975d
#)
pl_associate_package(
  PACKAGE_NAME physfs-3.0.2-rev1-linux
  TARGETS physfs
  PACKAGE_HASH a9d6d79554b08c962e0e715ef755256fa6f76979c548619e0d420fc799026f50
)
pl_associate_package(
  PACKAGE_NAME smolv-20200525-linux
  TARGETS smolv
  PACKAGE_HASH 15b7108f8ae2790893cdc5492eece6e681a5650f4091961c40f1b344506ef629
)
pl_associate_package(
  PACKAGE_NAME rapidjson-1.1.0-rev1-linux
  TARGETS rapidjson
  PACKAGE_HASH 920a417378e24924e1f8701e7fb282d7347af8a2e191e200445563307035d2dd
)
pl_associate_package(
PACKAGE_NAME glm-0.9.9.8-rev1-linux
TARGETS glm
PACKAGE_HASH e23fd40b8d61cc9d006e66d9d5c313abdd7d92037afb410e47b7a0329724b8e0
)
pl_associate_package(
  PACKAGE_NAME pcre-8.37-linux
  TARGETS pcre
  PACKAGE_HASH 1dbc14034d775a849714f47ab075eb8725dfbd1ce4fbe45230d6fb84582449e7
)
pl_associate_package(
  PACKAGE_NAME zlib-1.2.11-rev1-linux
  TARGETS zlib
  PACKAGE_HASH de286a2a2a9dffea15eccf3bc429077c223bf66bcfe0b07b9d94d2e1f16cede7
)
#pl_associate_package(
#  PACKAGE_NAME catch2-2.13.7-rev1-linux
#  TARGETS catch2
#  PACKAGE_HASH 02caeca67f7ce0bcb2ca616e4c9617a6b76f74630cd4d907a970a65073113e73
#)
pl_associate_package(
  PACKAGE_NAME mojoshader-master-rev1-linux
  TARGETS mojoshader
  PACKAGE_HASH 896b64489ab8b0a8bb94872700987296e523065d6a7c5c424f6f6c9f57787ba8
)
pl_associate_package(
  PACKAGE_NAME crunch-1.0.4-rev1-linux
  TARGETS crunch
  PACKAGE_HASH 44b8dd7b4c8e1eca7cc3ab0c1b8c03b93c6aa2d030b8430222c144c95cf792dd
)
pl_associate_package(
  PACKAGE_NAME ies-1.0.1-rev1-linux
  TARGETS ies
  PACKAGE_HASH e16144787532bdd2356ca57b626792662f63e3d043af0f45d5bdd01a0b1786ca
)
pl_associate_package(
  PACKAGE_NAME assimp-5.2.3-rev1-linux
  TARGETS assimp
  PACKAGE_HASH fb7af8d9dd35fdc3aab8412c57f32068e3bbebfc5c2cb0a0e29c3c6ae37cd6cb
)
pl_associate_package(
  PACKAGE_NAME meshoptimizer-0.17-rev1-linux
  TARGETS meshoptimizer
  PACKAGE_HASH 0ce658a533421adee07c13c9500172378e2cc41bb9460ea92fb44e557f83de68
)
pl_associate_package(
  PACKAGE_NAME mikktspace-1.0.0-rev1-linux
  TARGETS mikktspace
  PACKAGE_HASH 36c0fa7598da80bbe54002bdce49cfa546aec23f833c1f2cf4728dd4b76f1f3a
)
pl_associate_package(
  PACKAGE_NAME xsimd-7.4.8-rev1-linux
  TARGETS xsimd
  PACKAGE_HASH e041a75b00883d1f790299d4d3dff6cf1a828e16b39cfc0055a98793528fea39
)
pl_associate_package(
  PACKAGE_NAME imgui-1.87-rev1-linux
  TARGETS imgui
  PACKAGE_HASH 8445cc3f3ca2e3a9cfd850e963285a7cc2d37bf70ba1b53004e103f9ffac3f83
)
pl_associate_package(
  PACKAGE_NAME imgui_gizmo-1.8.3-rev1-linux
  TARGETS imgui_gizmo
  PACKAGE_HASH 6d85010645e00ec3b3aed2b1faa575db8063d2847ba4a5eab12f5dfa267ef24c
)
pl_associate_package(
  PACKAGE_NAME acl-2.0.1-rev1-linux
  TARGETS acl
  PACKAGE_HASH 1523a687056e6c749912802c8c76bf1461c73b881b0125389d3f72ec453d4324
)
pl_associate_package(
  PACKAGE_NAME rtm-2.1.3-rev1-linux
  TARGETS rtm
  PACKAGE_HASH 339c319fd77e9980034def573737f81379ca522fa04f947fd5f6a51e6cb4c023
)
pl_associate_package(
  PACKAGE_NAME mimalloc-2.0.6-rev1-linux
  TARGETS mimalloc
  PACKAGE_HASH d54b3f84f886690a0ffcc7013a971e75994b065e317a94041c66403b90e696a1
)