<script setup>
  const props = defineProps({
    console: {
      type: String,
      required: true
    },
    size: {
      type: String,
      default: '32',
      required: false
    }
  })
</script>

<template>
  <div>
    <img
      :src="`/platformIconsSprites/${props.console}.svg`"
      :width="size" :height="size"
      class="
        hover:scale-120
        transition-transform
            "
      :title="console"
    />
  </div>

</template>
