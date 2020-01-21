static const int depth = 8;
static const double angle = 15.0; // angle must be in degrees
static const double scale_factor = 0.8;
static const char background[] = "black";

#define gay
#ifdef gay
static char *colours[] = {
  "#FF0018",
  "#FFA52C",
  "#FFFF41",
  "#008018",
  "#0000F9",
  "#86007D"
};
#elif defined bi
static char *colours[] = {
  "#D60270",
  "#9B4F96",
  "#0038A8"
};
#else
static char *colours[] = {
  "#000000"
};
#endif
#define LENGTH 500
#define WIDTH 7
