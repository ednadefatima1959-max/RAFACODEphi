package com.vectras.vm.network;

import android.net.Uri;

public final class NetworkEndpoints {

    private static final String ROM_API_BASE = "https://go.anbui.ovh/egg/";
    private static final String GITHUB_API_BASE = "https://api.github.com/users/";
    private static final String GITHUB_PROFILE_BASE = "https://github.com/";
    private static final String LANGUAGE_MODULES_BASE = "https://raw.githubusercontent.com/rafaelmeloreisnovo/Vectras-VM-Android/main/resources/lang/";

    private NetworkEndpoints() {
    }

    public static String romContentInfo(String contentId, boolean isAnBuiId) {
        Uri.Builder builder = Uri.parse(ROM_API_BASE + "contentinfo").buildUpon()
                .appendQueryParameter("id", safeValue(contentId));
        if (!isAnBuiId) {
            builder.appendQueryParameter("app", "vectrasvm");
        }
        return EndpointPolicy.requireAllowedApi(EndpointPolicy.Feature.ROM_API, builder.build().toString());
    }

    public static String romUpdateLike() {
        String url = Uri.parse(ROM_API_BASE + "updatelike").buildUpon()
                .appendQueryParameter("app", "verctrasvm")
                .build()
                .toString();
        return EndpointPolicy.requireAllowedApi(EndpointPolicy.Feature.ROM_API, url);
    }

    public static String romUpdateView() {
        String url = Uri.parse(ROM_API_BASE + "updateview").buildUpon()
                .appendQueryParameter("app", "vectrasvm")
                .build()
                .toString();
        return EndpointPolicy.requireAllowedApi(EndpointPolicy.Feature.ROM_API, url);
    }

    public static String githubUserApi(String username) {
        String url = GITHUB_API_BASE + Uri.encode(safeValue(username));
        return EndpointPolicy.requireAllowedApi(EndpointPolicy.Feature.GITHUB_API, url);
    }

    public static String githubProfile(String username) {
        String url = GITHUB_PROFILE_BASE + Uri.encode(safeValue(username));
        return EndpointPolicy.requireAllowedActionView(EndpointPolicy.Feature.GITHUB_API, url);
    }

    public static String languageModule(String languageCode) {
        String url = LANGUAGE_MODULES_BASE + Uri.encode(safeValue(languageCode)) + ".json";
        return EndpointPolicy.requireAllowedApi(EndpointPolicy.Feature.LANG_MODULES, url);
    }

    public static String termuxReleasePage() {
        String url = "https://github.com/termux/termux-app/releases";
        return EndpointPolicy.requireAllowedActionView(EndpointPolicy.Feature.EXTERNAL_ACTIONS, url);
    }

    public static String termuxPulseAudioScript() {
        String url = "https://raw.githubusercontent.com/AnBui2004/termux/refs/heads/main/installpulseaudio.sh";
        return EndpointPolicy.requireAllowedActionView(EndpointPolicy.Feature.EXTERNAL_ACTIONS, url);
    }

    private static String safeValue(String value) {
        return value == null ? "" : value.trim();
    }
}
