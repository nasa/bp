/*
 * NASA Docket No. GSC-18,587-1 and identified as “The Bundle Protocol Core Flight
 * System Application (BP) v6.5”
 *
 * Copyright © 2020 United States Government as represented by the Administrator of
 * the National Aeronautics and Space Administration. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file
 *
 *  The BPNode header file containing version information
 */

#ifndef BPNODE_VERSION_H
#define BPNODE_VERSION_H


/* Development Build Macro Definitions */

#define BPNODE_BUILD_NUMBER    (9u) /*!< Development Build: Number of development builds (not commits) since baseline */
#define BPNODE_BUILD_BASELINE  "equuleus-rc1" /*!< Development Build: git tag that is the base for the current development */
#define BPNODE_BUILD_DEV_CYCLE "equuleus-rc2" /**< @brief Development: Release name for current development cycle */
#define BPNODE_BUILD_CODENAME  "Equuleus" /**< @brief: Development: Code name for the current build */

/*
** Macro Definitions
*/

#define BPNODE_MAJOR_VERSION 0  /*!< @brief Major version number. */
#define BPNODE_MINOR_VERSION 0  /*!< @brief Minor version number. */
#define BPNODE_REVISION      0  /*!< @brief Revision version number. Value of 0 indicates a development version.*/

/*!
 * @brief Mission revision.
 *
 * Reserved for mission use to denote patches/customizations as needed.
 * Values 1-254 are reserved for mission use to denote patches/customizations as needed. NOTE: Reserving 0 and 0xFF for
 * cFS open-source development use (pending resolution of nasa/cFS#440)
 */
#define BPNODE_MISSION_REV 0xFF

#define BPNODE_STR_HELPER(x) #x /*!< @brief Helper function to concatenate strings from integer macros */
#define BPNODE_STR(x) \
    BPNODE_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer macros */

/*! @brief Development Build Version Number.
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define BPNODE_VERSION BPNODE_BUILD_BASELINE "+dev" BPNODE_STR(BPNODE_BUILD_NUMBER)

/**
 * @brief Max Version String length.
 *
 * Maximum length that an OSAL version string can be.
 *
 */
#define BPNODE_CFG_MAX_VERSION_STR_LEN 256

#endif /* BPNODE_VERSION_H */
