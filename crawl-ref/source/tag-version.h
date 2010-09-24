#ifndef TAG_VERSION_H
#define TAG_VERSION_H

// Let CDO updaters know if the syntax changes.
#define TAG_MAJOR_VERSION  30

// Minor version will be reset to zero when major version changes.
enum tag_minor_version
{
    TAG_MINOR_RESET        = 0, // Minor tags were reset
    TAG_MINOR_REDUCED_LOS  = 1, // LOS radius reduced.
    TAG_MINOR_FEAR         = 2, // fear_mongers
    TAG_MINOR_VERSION      = 2, // Current version.  (Keep equal to max.)
};

#endif
