Name:       capi-ui-inputmethod
Summary:    Input Method Library
Version:    0.0.13
Release:    1
Group:      Graphics & UI Framework/Input
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(ecore-imf)
BuildRequires:  pkgconfig(libscl-core)
BuildRequires:  pkgconfig(isf)
BuildRequires:  pkgconfig(cynara-client)
BuildRequires:  pkgconfig(cynara-session)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Input Method Library


%package devel
Summary:  Input Method Library (Development)
Group:    Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Input Method Library (Development)



%prep
%setup -q


%build

export CFLAGS+=" -DTIZEN_DEBUG_ENABLE -fPIC -fvisibility=hidden -Werror"
export CXXFLAGS+=" -DTIZEN_DEBUG_ENABLE -fPIC -fvisibility=hidden -Werror"
export FFLAGS+=" -DTIZEN_DEBUG_ENABLE -fPIC -fvisibility=hidden"

%if "%{?profile}" == "wearable"
export CFLAGS="$CFLAGS -DTIZEN_WEARABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_WEARABLE"
export FFLAGS="$FFLAGS -DTIZEN_WEARABLE"
%endif

rm -rf CMakeFiles
rm -rf CMakeCache.txt
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER} -DLIB_INSTALL_DIR:PATH=%{_libdir} \
%if "%{?profile}" == "wearable"
    -DTIZEN_WEARABLE=YES \
%endif

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-ui-inputmethod.manifest
%{_libdir}/libcapi-ui-inputmethod.so*
/usr/share/license/capi-ui-inputmethod

%files devel
%{_includedir}/inputmethod*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-ui-inputmethod.so


