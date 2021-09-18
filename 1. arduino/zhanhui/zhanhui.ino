#include <MycobotBasic.h>
#include <SCServo.h>
SMSBL sm;
MycobotBasic myCobot;

typedef struct{
  int joint_angle{6};
} joint_angles_enc;

int data_len_max = 148;
joint_angles_enc jae[148];

int pos[149][6] = {{1141,2452,620,1157,1987,3985},
{1142,2452,620,1161,1993,3986},
{1142,2452,620,1162,1995,3986},
{1142,2452,620,1162,1993,3986},
{1142,2452,620,1163,1995,3986},
{1537,2215,499,1373,1826,3999},
{1527,2237,506,1341,1844,3999},
{1487,2311,546,1289,1871,3990},
{1461,2429,618,1217,1867,3986},
{1463,2573,698,1107,1855,3984},
{1472,2712,770,1032,1851,3984},
{1483,2805,820,1016,1838,3984},
{1488,2795,825,1041,1826,3984},
{1490,2715,790,1116,1824,3984},
{1490,2575,733,1223,1822,3984},
{1490,2405,684,1371,1829,3984},
{1491,2252,647,1505,1843,3984},
{1492,2138,633,1604,1857,3985},
{1520,2087,633,1614,1858,3994},
{1578,2112,634,1578,1880,4003},
{1626,2198,634,1483,1894,4007},
{1652,2332,635,1328,1901,4005},
{1659,2500,652,1153,1897,4005},
{1661,2661,701,1026,1890,4004},
{1661,2792,765,970,1884,4004},
{1665,2868,817,957,1884,4004},
{1667,2877,842,971,1891,4005},
{1667,2845,829,994,1895,4005},
{1668,2775,787,1042,1894,4004},
{1668,2666,716,1100,1884,4003},
{1668,2526,641,1178,1883,4002},
{1668,2370,591,1286,1902,4001},
{1668,2206,588,1476,1964,3998},
{1675,2066,622,1671,2000,3997},
{1732,1966,680,1824,2003,3997},
{1826,1959,742,1872,2015,4004},
{1918,2031,787,1851,2028,4006},
{1991,2163,824,1738,2037,4007},
{2041,2331,857,1591,2035,4007},
{2073,2513,898,1438,2016,4007},
{2090,2687,949,1302,1990,4008},
{2098,2831,1007,1210,1963,4012},
{2097,2923,1053,1195,1940,4017},
{2096,2940,1079,1204,1938,4036},
{2095,2913,1077,1223,1946,4072},
{2095,2849,1055,1284,1952,4080},
{2094,2754,1008,1363,1954,4080},
{2093,2638,945,1442,1940,4083},
{2090,2515,880,1518,1915,4087},
{2060,2398,816,1551,1904,4093},
{2019,2296,765,1567,1900,4094},
{1961,2204,730,1628,1887,4094},
{1872,2124,722,1718,1871,4086},
{1761,2039,729,1855,1826,4080},
{1645,1965,742,1992,1702,4080},
{1515,1907,752,2052,1606,4080},
{1385,1877,762,2050,1507,4076},
{1260,1869,771,1908,1460,4075},
{1145,1871,773,1713,1436,4075},
{1041,1871,775,1618,1351,4075},
{951,1871,777,1595,1273,4075},
{878,1871,777,1574,1247,4075},
{832,1871,777,1573,1243,4075},
{824,1871,774,1576,1254,4076},
{834,1871,768,1603,1320,4083},
{863,1871,768,1709,1408,4083},
{917,1871,768,1832,1477,4083},
{990,1871,768,1942,1540,4083},
{1074,1870,768,2024,1620,4083},
{1166,1870,767,2111,1727,4083},
{1266,1865,767,2170,1848,4083},
{1376,1867,767,2188,1944,4083},
{1488,1869,767,2185,2031,4083},
{1595,1869,768,2172,2132,4083},
{1694,1870,768,2133,2252,4083},
{1783,1869,769,2079,2337,4083},
{1868,1863,773,2019,2387,4083},
{1953,1862,776,1976,2449,4083},
{2026,1864,776,1951,2558,4081},
{2084,1865,777,1922,2668,4076},
{2134,1867,777,1889,2743,4076},
{2173,1863,778,1867,2794,4076},
{2214,1865,778,1836,2848,4075},
{2256,1871,778,1791,2903,4075},
{2293,1871,778,1741,2933,4074},
{2325,1870,778,1715,2937,4072},
{2353,1870,778,1698,2939,4070},
{2375,1869,778,1677,2945,4070},
{2387,1867,778,1674,2945,4070},
{2388,1869,778,1691,2934,4069},
{2386,1870,777,1720,2906,4068},
{2380,1871,775,1762,2875,4068},
{2340,1871,772,1792,2866,4068},
{2284,1871,771,1821,2836,4068},
{2227,1871,770,1877,2797,4068},
{2158,1871,769,1959,2718,4069},
{2077,1871,769,2037,2604,4076},
{1989,1871,769,2062,2487,4076},
{1890,1869,773,2011,2372,4076},
{1785,1864,773,1918,2272,4076},
{1684,1862,776,1808,2160,4076},
{1595,1861,777,1685,2101,4076},
{1527,1861,777,1594,2114,4072},
{1470,1859,778,1504,2110,4068},
{1418,1835,779,1441,2109,4052},
{1370,1790,784,1429,2117,4011},
{1322,1742,822,1473,2142,3974},
{1274,1710,895,1548,2167,3957},
{1230,1696,988,1631,2193,3953},
{1190,1700,1081,1708,2210,3952},
{1159,1713,1166,1773,2219,3952},
{1136,1728,1231,1829,2215,3952},
{1127,1745,1275,1880,2207,3952},
{1125,1752,1288,1905,2201,3952},
{1125,1756,1287,1937,2150,3952},
{1125,1758,1287,2079,2065,3951},
{1125,1759,1287,2226,2022,3945},
{1125,1760,1286,2304,1989,3929},
{1125,1781,1274,2356,1989,3922},
{1129,1825,1238,2378,1974,3921},
{1148,1868,1189,2377,1928,3921},
{1197,1907,1126,2348,1887,3921},
{1260,1946,1052,2302,1866,3922},
{1312,1981,968,2216,1866,3930},
{1347,2011,873,2094,1875,3931},
{1354,2048,777,1932,1880,3931},
{1353,2101,697,1772,1888,3931},
{1352,2166,643,1629,1891,3931},
{1352,2235,628,1532,1899,3931},
{1351,2303,628,1457,1898,3931},
{1351,2363,628,1359,1904,3931},
{1339,2406,628,1263,1931,3931},
{1299,2436,629,1179,1964,3926},
{1265,2443,629,1140,1970,3926},
{1236,2443,628,1128,1965,3920},
{1201,2450,625,1141,1969,3918},
{1156,2451,625,1167,1962,3918},
{1115,2451,627,1164,1961,3917},
{1096,2451,627,1163,1960,3915},
{1097,2451,625,1162,1963,3922},
{1111,2451,625,1162,1969,3924},
{1129,2451,622,1162,1972,3931},
{1135,2451,621,1162,1972,3937},
{1135,2451,621,1159,1973,3937},
{1135,2451,622,1159,1973,3938},
{1135,2451,622,1158,1973,3940},
{1136,2452,621,1156,1976,3953},
{1136,2452,621,1156,1980,3971},
{1138,2452,622,1155,1981,3982}};

void setup() {
  myCobot.setup();
  M5.Lcd.setTextSize(3);
  Serial.begin(9600);
  myCobot.powerOn();
  myCobot.setLEDRGB(0,255,0);
  Serial2.begin(1000000);
  sm.pSerial = &Serial2;

  for(int i = 0; i<6; i++){
      sm.WritePosEx(i+1,2048,2000,50);
      delay(50);
    }
}
void loop()
{
  play();
  delay(3000);
}
void play()  // is stop  is pause
{
  myCobot.setLEDRGB(0,255,0);
  for (int index = 0 ; index < 149; index++)
  {
    for(int i = 0; i<6; i++){
      sm.WritePosEx(i+1,pos[index][i],2000,50);
    }
    delay(100);
    Serial.println("Finish Play");
  }
}