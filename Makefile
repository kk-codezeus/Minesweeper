# ======================================================================
# FILE:        Makefile
# ======================================================================
#
RAW_SOURCES = \
	Main.cpp\
	MyAI.cpp\
	World.cpp

SOURCE_DIR = src
BIN_DIR = bin
DOC_DIR = doc
SOURCES = $(foreach s, $(RAW_SOURCES), $(SOURCE_DIR)/$(s))

all: $(SOURCES)
	@rm -rf $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DOC_DIR)
	@g++ -std=c++11 -g $(SOURCES) -o $(BIN_DIR)/Minesweeper

submission: all
	@rm -f *.zip
	@echo ""
	@read -p "Enter Team Name (No spaces, '_', '/', '*'): " teamName; \
	 echo ""; \
	 zip -rqq s_$${teamName}.zip $(SOURCE_DIR) $(BIN_DIR) $(DOC_DIR)
