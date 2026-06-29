# 🎮 P4 RPG PROJECT  
**Unreal Engine 5 / Gameplay Ability System 기반 오픈월드 RPG**

📸 최종 데모 영상
- https://youtu.be/tKVDAmAYPUU
---

## 🧩 프로젝트 개요

> 언리얼 엔진 5와 **Gameplay Ability System(GAS)** 을 활용해  
> 확장성과 유지보수성이 뛰어난 **오픈월드 RPG 전투 시스템**을 구축하는 프로젝트입니다.

- **개발 기간:** 2024.11.03 ~ 2024.11.21 (3주)
- **개발 엔진:** Unreal Engine 5.6 (C++ + Blueprint)
- **개발 언어:** C++
- **프로젝트 이름:** P4RPG
- **팀명:** PISA

---

## 🎯 프로젝트 목표

1. **GAS를 활용한 전투 구조 확립**  
   - 플레이어와 몬스터 모두 Ability / Effect / Attribute 구조를 사용  
   - 전투, 버프, 페이즈 전환을 GameplayTag 기반으로 제어  

2. **데이터 중심 설계 (Data-Driven Design)**  
   - 스탯, 아이템, 퀘스트, 스킬을 모두 DataTable / DataAsset으로 관리  
   - 코드 수정 없이 확장 가능한 구조 구현  

3. **팀 단위 협업 및 실무형 아키텍처 학습**  
   - Git, Notion, Trello를 통한 실시간 협업  
   - Unreal의 멀티모듈 구조 설계 및 코드 표준화  

4. **오픈월드 RPG의 기본 전투 루프 완성**  
   - “플레이어 ↔ 몬스터” 전투, 스탯 연동, 퀘스트, UI 연결  

---

## 🧠 팀 구성 및 역할

| 이름 | 역할 | 담당 업무 |
|------|------|------------|
| **이선우** | 🎮 Player Programmer | PlayerCharacter, CombatManager, GAS 연동, SkillTree, 입력 시스템 |
| **박현용** | 🧠 AI & Monster Programmer | MonsterBase, AIController, AttributeSet, Attack Delegate, Boss 페이즈 |
| **한승헌** | 🌍 World & Quest Developer | WorldTrigger, QuestManager, HUD, GameplayTag Event |
| **노현기** | 📦 Data & Inventory Engineer | ItemDB, Inventory, GAS 데이터 테이블, Save/Load 시스템 |

---

## 🕹️ 핵심 기능 요약

| 시스템 | 설명 |
|--------|------|
| **Ability System (GAS)** | 플레이어 & 몬스터 능력 / 효과 / 스탯 통합 구조 |
| **Combat Manager** | 공격, 콤보, 판정, 피격 처리 |
| **Skill Tree System** | 직업별 스킬 해금 및 AbilitySpec 등록 |
| **Monster AI System** | BehaviorTree 기반 패턴 / 페이즈 / Enrage 전환 |
| **Quest System** | 월드 트리거 / 퀘스트 진행 / 보상 연동 |
| **Data Management** | CSV / DataTable 기반 스탯, 스킬, 몬스터 관리 |
| **UI & HUD** | HP / 스킬 / 퀘스트 / 버프 HUD 실시간 갱신 |

---

## 🧱 개발 일정 (3주 계획)
| 주차                    | 목표                                       |
| --------------------- | ---------------------------------------- |
| **1주차 (11/3~11/7)**   | 플레이어, 몬스터, 인벤토리, 기본 UI 및 맵 생성 → 1차 데모 완성 |
| **2주차 (11/8~11/14)**  | 플레이어 스킬 시스템(GA), 보스 몬스터 AI, 퀘스트/아이템 확장   |
| **3주차 (11/15~11/21)** | 전체 통합, 최적화, 버그 수정, 최종 데모 발표              |

---

## 🔧 사용 기술

- **Unreal Engine 5.6**
- **C++ / Blueprint**
- **Gameplay Ability System (GAS)**
- **Behavior Tree**
- **DataTable / DataAsset**
- **Git / Notion / Trello (협업 툴)**


