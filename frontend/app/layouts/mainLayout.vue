<script setup lang="ts">
import MainSidebar from "~/components/MainSidebar.vue";
import {platforms} from "~/data/platforms"

const {platformFilter,statusFilter} = useGames();
const {searchQuery, showGrid, changeDisplayType} = mainLayout();
</script>

<template>
  <div class="h-screen flex overflow-hidden">
    <div class="flex flex-1">
      <MainSidebar />

      <div class="w-full">
        <UHeader>
          <template #left>
            <span>Filter</span>
            <UPopover arrow mode="click" :content="{align: 'center', side: 'bottom', sideOffset: 8}">
              <UButton icon="i-lucide:filter" color="neutral" variant="outline" size="xl"/>

              <template #content>
                <div class="flex flex-row">
                  <div class="flex flex-col">
                    <UCheckboxGroup class="pl-3 pr-3 pt-1 pb-1" legend="Platform" v-model="platformFilter" :items="platforms.map((item) => item.label)"/>
                    <UButton class="w-13 ml-3 mb-1.5" size="xs" label="All" variant="soft" @click="platformFilter = platforms.map((item) => item.label)"/>
                    <UButton class="w-13 ml-3 mb-1.5" size="xs" label="None" variant="soft" @click="platformFilter = []" />
                  </div>
                  <div class="flex flex-col">
                    <UCheckboxGroup class="pl-3 pr-3 pt-1 pb-1" v-model="statusFilter" legend="Status" :items="['Not Played','Started','Completed']"/>
                    <UButton class="w-13 ml-3 mb-1.5" size="xs" label="All" variant="soft" @click="statusFilter = ['Not Played','Started','Completed']"/>
                    <UButton class="w-13 ml-3 mb-1.5" size="xs" label="None" variant="soft" @click="statusFilter = []" />
                  </div>
                </div>
              </template>
            </UPopover>
          </template>

          <template #default>
            <UInput v-model="searchQuery" class="w-full" size="xl" placeholder="Search..." color="neutral" variant="outline" icon="i-lucide-search"/>
          </template>

          <template #right>
            <UButton @click="changeDisplayType" :icon="showGrid ? 'i-lucide:list' : 'i-lucide:layout-grid'" color="neutral" variant="outline" size="xl" />
            <UColorModeButton/>
          </template>
        </UHeader>

        <slot />
      </div>
    </div>
  </div>
</template>
