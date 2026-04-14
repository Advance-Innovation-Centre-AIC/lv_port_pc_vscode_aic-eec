# Metric Detail Structure (BP + Single Metric + Sleep)

This document reflects current metric-detail structure after implementing all metric pages.

## Navigation Flow

```mermaid
flowchart LR
    A[User Detail Cards] -->|Click Card| B[health_ui_root_open_metric_detail(metric_id)]
    B --> C[health_ui_metric_detail_to_page()]
    C --> D[Metric Detail Tab]
    D -->|Back in header| A
```

## Source of Truth

- Metric/page/title mapping: `src/ui/health/metric_detail_catalog.c`
- Root tab creation and routing: `src/ui/health/health_ui_root.c`
- Metric detail page dispatcher: `src/ui/health/pages/dashboard/page_metric_detail_layout.c`
- BP detail renderer: `src/ui/health/pages/dashboard/page_metric_bp_detail_layout.c`
- Single metric renderer (Glucose/Body Temp/SpO2/Weight):
  `src/ui/health/pages/dashboard/page_metric_single_detail_layout.c`
- Sleep renderer:
  `src/ui/health/pages/dashboard/page_metric_sleep_detail_layout.c`
- BP summary card component: `src/ui/health/components/content/comp_bp_metric_summary_card.c`
- BP detail sim snapshot: `src/ui/health/sim/health_ui_sim_data.c` (`health_ui_sim_data_build_bp_metric_detail`)
- Generic single metric sim snapshot:
  `health_ui_sim_data_build_single_metric_detail`
- Sleep sim snapshot:
  `health_ui_sim_data_build_sleep_metric_detail`
- Card click event -> metric route: `src/ui/health/pages/dashboard/page_user_detail_layout.c`

## Current Behavior

- Each metric has its own tab/page.
- BP detail page renders layout blocks from Figma:
  - top-right `Add Data` action in shared header row (`COMP_TAB_MENU_MODE_METRIC_DETAIL`)
  - date-range navigator row
  - 3 summary cards (SYS/DIA/PULSE)
  - line chart area with x-axis day labels and dual y-scale labels
- Glucose/Body Temp/SpO2/Weight pages use one shared single-metric renderer
  with one summary card + one line chart.
- Sleep page uses dedicated renderer with:
  - AVG metrics row (time in bed / time asleep)
  - sleep-stage timeline-style chart
  - legend summary row
- Header profile (avatar/name/age) stays synchronized across user-detail and metric pages.

## Next Phase

1. Bind sim snapshots to IPC source without changing renderer contracts.
2. Add period navigation behavior (prev/next) per metric.
3. Wire `Add Data` action to data-entry flow when backend is ready.
