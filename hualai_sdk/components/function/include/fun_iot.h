/*
 * @Description: header file of function layer
 * @Author: 
 * @Date: 2019-06-14 16:18:54
 * @LastEditTime: 2019-07-05 22:52:21
 */

#ifndef __FUN_IOT_H__
#define __FUN_IOT_H__



/**
 * @description:
 */
#define IOT_HOST_ADDR  "a1qoaolxz1s7tx.iot.cn-north-1.amazonaws.com.cn"
#define IOT_PORT       8883

#define IOT_CERTIFICATE_CRT "-----BEGIN CERTIFICATE-----\r\n"\
							"MIIDWjCCAkKgAwIBAgIVALdMXMisJcsSd+4t9afiFQjQBCifMA0GCSqGSIb3DQEB\r\n"\
						    "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\r\n"\
							"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODA5MjgwNzQy\r\n"\
							"MTZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\r\n"\
							"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDsEWLEr85mAH66IogL\r\n"\
							"oJ0ezh039/R2+sffpGUOkVwqKLf5uYulzvtKguk1paYhYdkQYuNttoDho2WKdkp+\r\n"\
							"iQfe6kWcmKqA4pXm8HOQwf1LDK+1ogL15R2HFOhLYYL53BqABUdFK3Apf+Uc2SMS\r\n"\
							"fjWds9vZyK91n7cJRVGuGsn7X+ARIRl1Jtv/9n4mKhfVU8Y7T6nTSEM93PIfAq5q\r\n"\
							"YcNaKlrXR8yCo+xsdfDuPZsngJOi/9V6uO43aRItQ8P/8yQ03oMdBjyS3CLZ1Z20\r\n"\
							"DhhdEu03TaOKN6tW3qsgwRls8d9UA2Bmyrv2xbztGviXCeY0Y/RrIRlkKkSfJ5Q7\r\n"\
							"XyajAgMBAAGjYDBeMB8GA1UdIwQYMBaAFM/cf6sV9GCS5bXFXzDLkf4OKMt8MB0G\r\n"\
							"A1UdDgQWBBRJdGA28EaQ9kKuOP2yfISRdMMo6TAMBgNVHRMBAf8EAjAAMA4GA1Ud\r\n"\
							"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAOiZNPMz/CFOVx3GHVFeHari0\r\n"\
							"SLCHEedq0Zl3whzTgiTja/GhBlFH98nT7mshhhJ+y5ee5jMTpqnZR6UmkttjIYZl\r\n"\
							"FXvRqcoYP2Bqp2L1JCua2KKxS2zZOHB/YAL6V/FB8B0AhrVBvTv55vQqXulYxEtZ\r\n"\
							"6Ak29vibiHbiIi7mZvh0qpd5B4khJmaZ6+s+bfRkCYKzhc0Ym34BGZ6+d/akeEka\r\n"\
							"Grbpd6zJoslECAyYBMMlXTMM23A5XGEzmlAVLhcG9K0VOpto2wGHtXJnYA09xULm\r\n"\
							"ljFJE5EYTUoEoUlkSA74mG8Si3mKxrsHsBGzKbTPl8Da208moYEJjB9ij7b09Q==\r\n"\
							"-----END CERTIFICATE-----\r\n"

#define IOT_PRIVATE_PEM_KEY "-----BEGIN RSA PRIVATE KEY-----\r\n"\
							"MIIEowIBAAKCAQEA7BFixK/OZgB+uiKIC6CdHs4dN/f0dvrH36RlDpFcKii3+bmL\r\n"\
							"pc77SoLpNaWmIWHZEGLjbbaA4aNlinZKfokH3upFnJiqgOKV5vBzkMH9SwyvtaIC\r\n"\
							"9eUdhxToS2GC+dwagAVHRStwKX/lHNkjEn41nbPb2civdZ+3CUVRrhrJ+1/gESEZ\r\n"\
							"dSbb//Z+JioX1VPGO0+p00hDPdzyHwKuamHDWipa10fMgqPsbHXw7j2bJ4CTov/V\r\n"\
							"erjuN2kSLUPD//MkNN6DHQY8ktwi2dWdtA4YXRLtN02jijerVt6rIMEZbPHfVANg\r\n"\
							"Zsq79sW87Rr4lwnmNGP0ayEZZCpEnyeUO18mowIDAQABAoIBAHPxZg0sO7clbxiE\r\n"\
							"dUUgsYq7uOYQRmVZbmmNGIU7J4UdUp64XrolGo7SFTBgF2Q15qKBrynnhQ1Nb0W+\r\n"\
							"K4BSVSdK8L2kzblXkEBIwLnMTVda98HBvKqAySvbPOwpDGcjj3BpJ0P1vbTkN+cf\r\n"\
							"TnZ6vmKiP8yLIIIo24EWJrPYPHPVBXRjYJEzVUqT1ujc3RbkfGzaGkulOQIy18Wg\r\n"\
							"6DSwmG4gkOLVVQxalT5ZhXsTK7yCSwrZ7/MrPcJYSFpKDXXT5Lw/1fcIwS2qdp8Z\r\n"\
							"FxCxeXRCZzxbUqtBC1DMpTKjzvhzmmlg1QsXYl+Ffyq+D3ek0+fV8HVrLdjdpW/O\r\n"\
							"QveZN2ECgYEA+s/I88apHVzZXaXQSABFhQmkLLFlOhbkMrMAdjKk03imOz3z/u8Y\r\n"\
							"3RCave6ogpD+ngG1N1VnbnHVVGauwL7+ScKR2Kakd7Z4hfmT6usnMT+tWj/G+qXT\r\n"\
							"QSYp1+rXqyQUjPkGTXxWcYY9vXQugoWPSiC5itJV8zuHzsDBQxcUtq8CgYEA8POF\r\n"\
							"2uJpNJW+R8DOU+Pugrfmqv4yRufze5/bzn13SgylkRzjgPT9Wgxtw3T3sb0c1Efj\r\n"\
							"jYfRcap86Oz2G7gNzmNf5SqlJb6NIGkmk4/bVmpCI2iBe2EBpYHMWdFHbPpsvoMc\r\n"\
							"Y/npeoSTFE7bZE2SUGNf52QVBmCIxNlgJ6ysDE0CgYEAnNm/CTzu2+8J/LfKpoyM\r\n"\
							"7WMXKeAStx5DvbUWD9UiISHti1T2NzMdsHpG24raFTVKFlRpGN6vHXHRAMRqex22\r\n"\
							"dzhX705fwoH5o4duStaenvz8k1kZPfzgPXj5aA90qpGYzISbvW+Ju9tA1y9GOtf9\r\n"\
							"wVTv0/IwUDmld9Yxxpwzlv8CgYA7Zg0tdHsy92c9EQ7MJ3h4yRUYOinyyOOBKltp\r\n"\
							"gAyUOKHoMBL1i2+Zb07vzDnzmwcsXbx++2hqePjqAuB43Nqqcl0FhYnI0DzCLKnR\r\n"\
							"3rjXiMJdMDobTEK3plPWIH1XJDUCkhfBna4tAdmwO/Bhva3AG39c75MvS0TsIhpO\r\n"\
							"LYcMUQKBgGvLqpTmmeEmR31u5E71MckkV8CMZY3xLMluqv7FHM+bvP+8F168TRL4\r\n"\
							"pBe5dfXz0jZ0kx4FWPIbjO6WLcyCh0pyxY2Vkp2WKSTJ5MBdqn8jY4GfryOGupQu\r\n"\
							"vM43+K8rebj3NpxXGIlCB4t8Wjkbz/9clStHiQ5S78bOr9wNB9OZ\r\n"\
							"-----END RSA PRIVATE KEY-----\r\n"

#define IOT_AWS_ROOT_PEM    "-----BEGIN CERTIFICATE-----\r\n"\
							"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"\
							"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"\
							"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"\
							"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"\
							"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"\
							"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"\
							"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"\
							"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"\
							"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"\
							"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"\
							"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"\
							"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"\
							"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"\
							"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"\
							"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"\
							"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"\
							"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"\
							"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"\
							"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"\
							"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"\
							"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"\
							"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"\
							"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"\
							"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"\
							"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"\
							"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"\
							"-----END CERTIFICATE-----\r\n"


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_iot_work_function(void);


#endif
/*************************************************END****************************************************/