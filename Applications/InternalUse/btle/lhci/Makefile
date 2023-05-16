VERSION=08

all: 
	mkdir max32665_v${VERSION}
	cp IUT_source/build/max32665_hci.elf max32665_v${VERSION}
	cp README.txt max32665_v${VERSION}
	# cp MAX32665_BLE_Bug_Report.pdf max32665_v${VERSION}
	zip -r max32665_v${VERSION}.zip max32665_v${VERSION}
	cp max32665_v${VERSION}.zip archive