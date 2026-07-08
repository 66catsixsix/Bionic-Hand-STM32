# LVGL UI Simulator

This folder is a desktop-only screenshot simulator for the current 800x480 LVGL UI.
It does not modify the embedded project files.

## Build

```sh
cd lvgl_ui_simulator
make
```

## Run interactively

```sh
make run
```

Use the mouse to click the same mode cards as the real touch screen. Press `q` or `Esc` to exit.

## Generate screenshots

```sh
make screenshots
```

This creates:

- `screenshots/home.bmp`
- `screenshots/free.bmp`
- `screenshots/rehab.bmp`
- `screenshots/emg.bmp`
- `screenshots/calib.bmp`

If ImageMagick `convert` is installed, PNG copies are created next to the BMP files.

For one page only:

```sh
make screenshot PAGE=rehab
```

To convert existing BMP screenshots to PNG:

```sh
make png
```
