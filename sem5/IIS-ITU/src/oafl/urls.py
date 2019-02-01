from .settings import DEBUG_TOOLBAR

from django.contrib import admin
from django.urls import path, include


urlpatterns = [
    path('', include('oafl.browser.urls')),
    path('admin/', admin.site.urls),
    path('accounts/', include('django.contrib.auth.urls')),
]

if DEBUG_TOOLBAR:
    import debug_toolbar

    urlpatterns += [
        path('__debug__/', include(debug_toolbar.urls)),
    ]
