CC ?= gcc

LVGL_ROOT := ../my_drivers/LVGL
TARGET := lvgl_ui_sim
PAGE ?= home

APP_SRCS := \
	src/main.c \
	src/stubs.c \
	src/ui/main_screen.c

LVGL_SRCS := $(shell find $(LVGL_ROOT)/src -type f -name '*.c' \
	! -path '*/draw/sdl/*' \
	! -path '*/draw/stm32_dma2d/*' \
	! -path '*/draw/arm2d/*' \
	! -path '*/draw/nxp/*' \
	! -path '*/draw/swm341_dma2d/*')

CFLAGS += -std=c11 -O2 -Wall -Wextra -Wno-unused-parameter -DLV_CONF_INCLUDE_SIMPLE
CFLAGS += -Iinclude -Isrc/ui -I$(LVGL_ROOT) -I$(LVGL_ROOT)/src
CFLAGS += $(shell pkg-config --cflags sdl2)
LDLIBS += $(shell pkg-config --libs sdl2)

.PHONY: all run screenshot screenshots png clean

all: $(TARGET)

$(TARGET): $(APP_SRCS) $(LVGL_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

screenshot: $(TARGET)
	mkdir -p screenshots
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page $(PAGE) --screenshot screenshots/$(PAGE).bmp

screenshots: $(TARGET)
	mkdir -p screenshots
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page home --screenshot screenshots/home.bmp
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page free --screenshot screenshots/free.bmp
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page rehab --screenshot screenshots/rehab.bmp
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page emg --screenshot screenshots/emg.bmp
	SDL_VIDEODRIVER=dummy ./$(TARGET) --page calib --screenshot screenshots/calib.bmp
	$(MAKE) png

png:
	@if command -v convert >/dev/null 2>&1; then \
		for f in screenshots/*.bmp; do convert "$$f" "$${f%.bmp}.png"; done; \
		echo "PNG screenshots written to screenshots/*.png"; \
	else \
		echo "ImageMagick convert not found; BMP screenshots are still available."; \
	fi

clean:
	rm -f $(TARGET)
