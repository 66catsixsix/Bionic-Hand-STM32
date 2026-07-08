# Calibration Decision Log

## Logging Rule
- 这里只记录“确认可复现”的结论和当前生效参数。
- 日常临时测试波动、一次性串口片段不入库。

## Active Baseline (2026-04-25)

### PA0 Root (`ch=0`, `Finger_Index`)
- Selected calibration:
  - `open1=2008`, `close=1858`, `open2=1973`
- Effective refs:
  - `adc_open_ref=1991`
  - `adc_close_ref=1858`
- Quality status:
  - usable, average quality (return offset exists)
- Known behavior:
  - full bend sometimes does not feel like hard-close endpoint.

### PA2 Root (`ch=2`, `Finger_Middle`)
- Selected calibration:
  - `open1=2068`, `close=1929`, `open2=2010`
- Effective refs:
  - `adc_open_ref=2039`
  - `adc_close_ref=1929`
- Quality status:
  - usable, average quality (small return hysteresis)
- Known behavior:
  - both endpoints reachable, mid-range slightly sensitive.

## Next Validation Focus
- Dual-root concurrent control (`PA0 + PA2`) stability.
- If both roots show same mid-range issue, optimize mapping/filter globally.
