# barrier for searching Android.mk
# Many of this folder's subfolders are duplicates of CM's "android/system/external" folder.
# Some of them appear to have Mediatek specific customizations; most of them do not.
# Regardless, CM's "make" process will enumerate this folder and see conflicts.
# Having this Android.mk file here prevents "make" from enumerating subfolders.
