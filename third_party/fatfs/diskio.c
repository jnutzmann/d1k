/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "fatfs/diskio.h"
#include "fatfs/ff.h"

/* Not USB in use */
/* Define it in defines.h project file if you want to use USB */
#ifndef FATFS_USE_USB
	#define FATFS_USE_USB				0
#endif

/* Not SDRAM in use */
/* Define it in defines.h project file if you want to use SDRAM */
#ifndef FATFS_USE_SDRAM
	#define FATFS_USE_SDRAM				0
#endif

/* Not SPI FLASH in use */
/* Define it in defines.h project file if you want to use SPI FLASH */
#ifndef FATFS_USE_SPI_FLASH
	#define FATFS_USE_SPI_FLASH			0
#endif

/* Set in defines.h file if you want it */
#ifndef TM_FATFS_CUSTOM_FATTIME
	#define TM_FATFS_CUSTOM_FATTIME		0
#endif

/* Defined in defines.h */
/* We are using FATFS with USB */
#if FATFS_USE_USB == 1 || FATFS_USE_SDRAM == 1 || FATFS_USE_SPI_FLASH == 1
	/* If SDIO is not defined, set to 2, to disable SD card */
	/* You can set FATFS_USE_SDIO in defines.h file */
	/* This is for error fixes */
	#ifndef FATFS_USE_SDIO
		#define FATFS_USE_SDIO			2
	#endif
#else
	/* If USB is not used, set default settings back */
	/* By default, SDIO is used */
	#ifndef FATFS_USE_SDIO
		#define FATFS_USE_SDIO			1
	#endif
#endif

/* USB with FATFS */
#if FATFS_USE_USB == 1
	#include "fatfs_usb.h"
#endif /* FATFS_USE_USB */
/* SDRAM with FATFS */
#if FATFS_USE_SDRAM == 1
	#include "fatfs_sdram.h"
#endif /* FATFS_USE_SDRAM */
/* SPI FLASH with FATFS */
#if FATFS_USE_SPI_FLASH == 1
	#include "fatfs_spi_flash.h"
#endif /* FATFS_USE_SPI_FLASH */

/* Include SD card files if is enabled */
#if FATFS_USE_SDIO == 1
	#include "fatfs_sdio_driver.h"
#elif FATFS_USE_SDIO == 0
	#include "fatfs_sd.h"
#endif


/* Definitions of physical drive number for each media */
#define ATA		   0
#define USB		   1
#define SDRAM      2
#define SPI_FLASH  3

/* Make driver structure */
DISKIO_LowLevelDriver_t FATFS_LowLevelDrivers[_VOLUMES] = {
	{
		fatfs_sd_sdio_disk_initialize,
		fatfs_sd_sdio_disk_status,
		fatfs_sd_sdio_disk_ioctl,
		fatfs_sd_sdio_disk_write,
		fatfs_sd_sdio_disk_read
	},
	{
		fatfs_usb_disk_initialize,
		fatfs_usb_disk_status,
		fatfs_usb_disk_ioctl,
		fatfs_usb_disk_write,
		fatfs_usb_disk_read
	},
	{
		fatfs_sdram_disk_initialize,
		fatfs_sdram_disk_status,
		fatfs_sdram_disk_ioctl,
		fatfs_sdram_disk_write,
		fatfs_sdram_disk_read
	},
	{
		fatfs_spi_flash_disk_initialize,
		fatfs_spi_flash_disk_status,
		fatfs_spi_flash_disk_ioctl,
		fatfs_spi_flash_disk_write,
		fatfs_spi_flash_disk_read
	}
};

void fatfs_add_driver(DISKIO_LowLevelDriver_t* driver, fatfs_driver_t driver_name) {
	if (
		driver_name != FATFS_DRIVER_USER1 &&
		driver_name != FATFS_DRIVER_USER2
	) {
		/* Return */
		return;
	}
	
	/* Add to structure */
	FATFS_LowLevelDrivers[(uint8_t) driver_name].disk_initialize = driver->disk_initialize;
	FATFS_LowLevelDrivers[(uint8_t) driver_name].disk_status = driver->disk_status;
	FATFS_LowLevelDrivers[(uint8_t) driver_name].disk_ioctl = driver->disk_ioctl;
	FATFS_LowLevelDrivers[(uint8_t) driver_name].disk_read = driver->disk_read;
	FATFS_LowLevelDrivers[(uint8_t) driver_name].disk_write = driver->disk_write;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	/* Return low level status */
	if (FATFS_LowLevelDrivers[pdrv].disk_initialize) {
		return FATFS_LowLevelDrivers[pdrv].disk_initialize();
	}
	
	/* Return parameter error */
	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	/* Return low level status */
	if (FATFS_LowLevelDrivers[pdrv].disk_status) {
		return FATFS_LowLevelDrivers[pdrv].disk_status();
	}
	
	/* Return parameter error */
	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	/* Check count */
	if (!count) {
		return RES_PARERR;
	}
	
	/* Return low level status */
	if (FATFS_LowLevelDrivers[pdrv].disk_read) {
		return FATFS_LowLevelDrivers[pdrv].disk_read(buff, sector, count);
	}
	
	/* Return parameter error */
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	/* Check count */
	if (!count) {
		return RES_PARERR;
	}
	
	/* Return low level status */
	if (FATFS_LowLevelDrivers[pdrv].disk_write) {
		return FATFS_LowLevelDrivers[pdrv].disk_write(buff, sector, count);
	}
	
	/* Return parameter error */
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	/* Return low level status */
	if (FATFS_LowLevelDrivers[pdrv].disk_ioctl) {
		return FATFS_LowLevelDrivers[pdrv].disk_ioctl(cmd, buff);
	}
	
	/* Return parameter error */
	return RES_PARERR;
}
#endif

/*-----------------------------------------------------------------------*/
/* Get time for fatfs for files                                          */
/*-----------------------------------------------------------------------*/
__weak DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}

/* Function declarations to prevent link errors if functions are not found */
__weak DSTATUS fatfs_sd_sdio_disk_initialize(void) {return RES_ERROR;}
__weak DSTATUS fatfs_usb_disk_initialize(void) {return RES_ERROR;}
__weak DSTATUS fatfs_sdram_disk_initialize(void) {return RES_ERROR;}
__weak DSTATUS fatfs_spi_flash_disk_initialize(void) {return RES_ERROR;}

__weak DSTATUS fatfs_sd_sdio_disk_status(void) {return RES_ERROR;}
__weak DSTATUS fatfs_usb_disk_status(void) {return RES_ERROR;}
__weak DSTATUS fatfs_sdram_disk_status(void) {return RES_ERROR;}
__weak DSTATUS fatfs_spi_flash_disk_status(void) {return RES_ERROR;}

__weak DRESULT fatfs_sd_sdio_disk_ioctl(BYTE cmd, void *buff) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_usb_disk_ioctl(BYTE cmd, void *buff) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_sdram_disk_ioctl(BYTE cmd, void *buff) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_spi_flash_disk_ioctl(BYTE cmd, void *buff) {return (DRESULT)STA_NOINIT;}

__weak DRESULT fatfs_sd_sdio_disk_read(BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_usb_disk_read(BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_sdram_disk_read(BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_spi_flash_disk_read(BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}

__weak DRESULT fatfs_sd_sdio_disk_write(const BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_usb_disk_write(const BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_sdram_disk_write(const BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
__weak DRESULT fatfs_spi_flash_disk_write(const BYTE *buff, DWORD sector, UINT count) {return (DRESULT)STA_NOINIT;}
