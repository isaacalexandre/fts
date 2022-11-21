include version

.DEFAULT_GOAL:=all

OUTPUT_DIR 	= output/
PLATFORMS 	= 64 86
SYSTEMS		= Client Server

PLATFORM 	?= 64
SYSTEMS		?= Client

.NOTPARALLEL:
.PHONY: all clean $(SYSTEMS) $(PLATFORMS) 
all: $(SYSTEMS)

$(SYSTEMS):
	@$(MAKE) --no-print-directory $(PLATFORMS) SYSTEM=$@

$(PLATFORMS):
	@$(MAKE) -B --no-print-directory -C src -f $(SYSTEM).mk PLATFORM=$@ clean all
	@mkdir -p $(OUTPUT_DIR)$(SYSTEM)/$@
	@rm -rf $(OUTPUT_DIR)$(SYSTEM)/$@/*
	@echo "Coping to $(OUTPUT_DIR)$(SYSTEM)/$@"
	@cp src/appfts$(SYSTEM) src/*.so $(OUTPUT_DIR)$(SYSTEM)/$@

clean:
	@$(MAKE) -C src -f Client.mk clean
	@$(MAKE) -C src -f Server.mk clean
	@rm -rf $(OUTPUT_DIR)
