<script setup lang="ts">
import type {NavigationMenuItem} from "@nuxt/ui/components/NavigationMenu.vue";

const items = ref<NavigationMenuItem[]>([
  {
    label: 'Home',
    icon: 'i-lucide-home'
  },
  {
    label: 'Games',
    icon: 'i-lucide-gamepad-2',
    defaultOpen: true,
    children: [
      {
        label: 'Library'
      },
      {
        label: 'Backlog'
      }
    ]
  },
  {
    label: 'Books',
    icon: 'i-lucide-book',
    defaultOpen: true,
    children: [
      {
        label: 'Library'
      },
      {
        label: 'Backlog'
      }
    ]
  }
])
</script>

<template>
  <USidebar
    side="left"
  >
    <template #default>
      <UNavigationMenu
        class="pt-10"
        orientation="vertical"
        :items="items"
        :ui="{
              linkLabel: 'text-xl',
              item: 'first:mb-20 mb-5',
              linkLeadingIcon: 'size-6',
              linkTrailingIcon: 'size-6'
            }"
      />
    </template>

    <template #footer>
      <UAvatar icon="i-lucide:circle-user-round" size="2xl" />
      <span class="pl-10 text-xl">Profile</span>
    </template>
  </USidebar>
</template>
