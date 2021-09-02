const { Discord, MessageEmbed } = require('discord.js');
const ready = require("../../configs/ready.json");
const si = require("systeminformation");

module.exports = async(bot) => {
    console.log(`${bot.user.tag} is now online.`)
    console.log(`Platformo: ${(await si.osInfo()).platform}\nSystem: ${(await si.osInfo()).distro}`);
    bot.user.setPresence({ activity: { name: ready.status }, status: 'online' })
}